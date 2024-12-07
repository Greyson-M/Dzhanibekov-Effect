#include "PhysObject.h"

PhysObject::PhysObject(Model model) : model(model)
{
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	com = glm::vec3(0.0f, 0.0f, 0.0f);

	omega = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::float32 volume = 0.0f;

	compute_inertia_tensor(1.0f, &I, &com, &mass, &volume);

	I_inv = glm::inverse(I);
	L = glm::vec3(0.0f, 3.0f, 0.1f);

	//translate to COMs
	glm::vec3 curr_pos = model.getTranslation();
	model.setTranslation(curr_pos - com);

}

PhysObject::PhysObject(const char* model_path) : model(model_path)
{
	mass = 1.0f;

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	com = glm::vec3(0.0f, 0.0f, 0.0f);

	omega = glm::vec3(1.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

PhysObject::~PhysObject()
{
}

static glm::mat3 cross_mat(glm::vec3 v)
{
	return glm::mat3(0.0f, -v.z, v.y,
					v.z, 0.0f, -v.x,
					-v.y, v.x, 0.0f);
}

void PhysObject::update()
{
	//glm::quat q = model.getOrientation();
	//glm::mat3 R = model.getOrientation_mat();
	glm::mat3 R = glm::mat3_cast(glm::normalize(model.getOrientation()));

	//glm::mat3 omega_cross = cross_mat(omega);
	//glm::mat3 R_dot = omega_cross * R;

	//glm::mat3 R_prime = R + Settings().dt * R_dot;

	//ensure r_prime will be orthonormal

	glm::mat3 R_T = glm::transpose(R);


	omega = R * I_inv * R_T * L;
	glm::quat q = model.getOrientation();
	glm::quat q_dot = 0.5f * glm::quat(0.0f, omega.x, omega.y, omega.z) * q;

	//glm::mat3 R_prime = R + Settings().dt * cross_mat(omega) * R;

	glm::quat q_prime = glm::normalize(q + Settings().dt * q_dot);

	//R_prime = glm::orthonormalize(R_prime);

	glm::mat3 ortho_check = R * R_T;

	//model.setOrientation(R_prime);
	model.setOrientation(q_prime);

	if (model.getTranslation().y < Settings().floor_height)
	{
		model.setTranslation(glm::vec3(model.getTranslation().x, Settings().floor_height, model.getTranslation().z));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity += acceleration * Settings().dt;
	model.setTranslation(model.getTranslation() + velocity * Settings().dt);
	com += velocity * Settings().dt;

}

glm::float32 PhysObject::ComputeInertiaMoment(glm::vec3* P, int I){

	glm::float32 result = (P[0][I] * P[0][I]) + (P[1][I] * P[2][I])
						+ (P[1][I] * P[1][I]) + (P[0][I] * P[2][I])
						+ (P[2][I] * P[2][I]) + (P[0][I] * P[1][I]);

	return result;
}

glm::float32 PhysObject::ComputeInertiaProduct(glm::vec3* P, int I, int J)
{
	glm::float32 result = 2.0f * (P[0][I] * P[0][J]) + (P[1][I] * P[2][J]) + (P[2][I] * P[1][J])
						+ 2.0f * (P[1][I] * P[1][J]) + (P[0][I] * P[2][J]) + (P[2][I] * P[0][J])
						+ 2.0f * (P[2][I] * P[2][J]) + (P[0][I] * P[1][J]) + (P[1][I] * P[0][J]);

	return result;
}

struct tri {
	glm::vec3 P[3];
};

void PhysObject::compute_inertia_tensor(glm::float32 density, glm::mat3* I0_out, glm::vec3* CoM_out, glm::float32* M_out, glm::float32* Vol_out)
{
	Mesh* mesh = model.getMesh(0);
	
	glm::float32 V = 0.0f;
	glm::float32 mass = 0.0f;
	glm::vec3 MassCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::float32 Ia = 0.0f, Ib = 0.0f, Ic = 0.0f, Iap = 0.0f, Ibp = 0.0f, Icp = 0.0f;

	std::vector<Vertex> vertices = mesh->vertices;
	std::vector<GLuint> indices = mesh->indices;

	std::vector<tri> faces = std::vector<tri>();

	//assemble the faces from vertices and indices
	for (int i = 0; i < indices.size(); i += 3)
	{
		tri face;
		for (int j = 0; j < 3; j++)
		{
			face.P[j] = vertices[indices[i + j]].position;
		}
		faces.push_back(face);
	}

	for (int i = 0; i < faces.size(); i++)
	{
		glm::vec3 P[3];

		P[0] = faces[i].P[2];
		P[1] = faces[i].P[1];
		P[2] = faces[i].P[0];

		//for (int j = 0; j < 3; j++)
		//{
		//	glm::vec3 localP;

		//}

		glm::float32 detJ = glm::dot(P[0], glm::cross(P[1], P[2]));
		glm::float32 tetraVol = detJ / 6.0f;
		glm::float32 tetraMass = density * tetraVol;
		glm::vec3 tetraCoM = (P[0] + P[1] + P[2]) / 4.0f;

		Ia += detJ * (ComputeInertiaMoment(P, 1) + ComputeInertiaMoment(P, 2));
		Ib += detJ * (ComputeInertiaMoment(P, 0) + ComputeInertiaMoment(P, 2));
		Ic += detJ * (ComputeInertiaMoment(P, 0) + ComputeInertiaMoment(P, 1));
		Iap += detJ * (ComputeInertiaProduct(P, 1, 2));
		Ibp += detJ * (ComputeInertiaProduct(P, 0, 1));
		Icp += detJ * (ComputeInertiaProduct(P, 0, 2));

		MassCenter += tetraMass * tetraCoM;
		mass += tetraMass;
		V += tetraVol;
	}

	MassCenter = MassCenter / mass;
	Ia = Ia * density / 60.0f - mass * (MassCenter.y * MassCenter.y + MassCenter.z * MassCenter.z);
	Ib = Ib * density / 60.0f - mass * (MassCenter.x * MassCenter.x + MassCenter.z * MassCenter.z);
	Ic = Ic * density / 60.0f - mass * (MassCenter.x * MassCenter.x + MassCenter.y * MassCenter.y);

	Iap = Iap * density / 120.0f - mass * MassCenter.y * MassCenter.z;
	Ibp = Ibp * density / 120.0f - mass * MassCenter.x * MassCenter.y;
	Icp = Icp * density / 120.0f - mass * MassCenter.x * MassCenter.z;

	glm::mat3 I = glm::mat3(Ia, -Iap, -Ibp,
							-Iap, Ib, -Icp,
							-Ibp, -Icp, Ic);

	*I0_out = I;
	*CoM_out = MassCenter;
	*M_out = mass;
	*Vol_out = V;
}
