#include "PhysObject.h"

PhysObject::PhysObject(Model model) : model(model)
{
	acceleration = glm::f64vec3(0.0, 0.0, 0.0);
	velocity = glm::f64vec3(0.0, 0.0, 0.0);
	com = glm::f64vec3(0.0, 0.0, 0.0);

	omega = glm::f64vec3(0.0, 0.0, 0.0);

	glm::float64 volume = 0.0;

	compute_inertia_tensor(1.0f, &I, &com, &mass, &volume);

	I_inv = glm::inverse(I);
	//L = glm::f64vec3(0.0, 0.01, 3.0);			//top
	//L = glm::f64vec3(0.02, 0.0001, 0.0);		//test2
	L = glm::f64vec3(-25.0, 0.0, 0.1);			//test

	//translate to COMs
	glm::f64vec3 curr_pos = model.getTranslation();
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

static glm::mat3 cross_mat(glm::f64vec3 v)
{
	return glm::f64mat3(0.0f, -v.z, v.y,
					v.z, 0.0f, -v.x,
					-v.y, v.x, 0.0f);
}

void PhysObject::update()
{
	//glm::quat q = model.getOrientation();
	//glm::mat3 R = model.getOrientation_mat();
	glm::f64mat3 R = glm::mat3_cast(glm::normalize(model.getOrientation()));

	//glm::mat3 omega_cross = cross_mat(omega);
	//glm::mat3 R_dot = omega_cross * R;

	//glm::mat3 R_prime = R + Settings().dt * R_dot;

	//ensure r_prime will be orthonormal

	glm::f64mat3 R_T = glm::transpose(R);

	glm::f64mat3 otho_check = R * R_T;


	omega = R * I_inv * R_T * L;
	glm::f64quat q = model.getOrientation();
	glm::f64quat q_dot = 0.5 * glm::f64quat(0.0f, omega.x, omega.y, omega.z) * q;

	//glm::mat3 R_prime = R + Settings().dt * glm::matrixCross3(omega) * R;
	//glm::vec3 omega_cross_Rx = glm::cross(omega, R[0]);
	//glm::vec3 omega_cross_Ry = glm::cross(omega, R[1]);
	//glm::vec3 omega_cross_Rz = glm::cross(omega, R[2]);
	//
	//glm::mat3 R_prime = R + Settings().dt * glm::mat3(omega_cross_Rx, omega_cross_Ry, omega_cross_Rz);

	//glm::mat3 R_prime = R + Settings().dt * glm::cross(omega, R);

	//glm::quat q_prime = glm::normalize(q + Settings().dt * q_dot);
	
	//rk4 integration of q
	glm::f64quat k1 = 0.5 * glm::f64quat(0.0f, omega.x, omega.y, omega.z) * q;
	glm::f64quat k2 = 0.5 * glm::f64quat(0.0f, omega.x, omega.y, omega.z) * (q + 0.5f * Settings().dt * k1);
	glm::f64quat k3 = 0.5 * glm::f64quat(0.0f, omega.x, omega.y, omega.z) * (q + 0.5f * Settings().dt * k2);
	glm::f64quat k4 = 0.5 * glm::f64quat(0.0f, omega.x, omega.y, omega.z) * (q + Settings().dt * k3);

	glm::f64quat q_prime = q + (Settings().dt / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);

	//R_prime = glm::orthonormalize(R_prime);

	//glm::mat3 ortho_checkprime = R_prime * glm::transpose(R_prime);

	//model.setOrientation(R_prime);
	model.setOrientation(q_prime);

	if (model.getTranslation().y < Settings().floor_height)
	{
		model.setTranslation(glm::vec3(model.getTranslation().x, Settings().floor_height, model.getTranslation().z));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity += acceleration * (float)Settings().dt;
	model.setTranslation(model.getTranslation() + velocity * (float)Settings().dt);
	com += velocity * (float)Settings().dt;
}

glm::f64vec3 PhysObject::get_omega()
{
	return omega;
}

glm::float64 PhysObject::ComputeInertiaMoment(glm::f64vec3* P, int I){

	glm::float64 result = (P[0][I] * P[0][I]) + (P[1][I] * P[2][I])
						+ (P[1][I] * P[1][I]) + (P[0][I] * P[2][I])
						+ (P[2][I] * P[2][I]) + (P[0][I] * P[1][I]);

	return result;
}

glm::float64 PhysObject::ComputeInertiaProduct(glm::f64vec3* P, int I, int J)
{
	glm::float64 result = 2.0f * (P[0][I] * P[0][J]) + (P[1][I] * P[2][J]) + (P[2][I] * P[1][J])
						+ 2.0f * (P[1][I] * P[1][J]) + (P[0][I] * P[2][J]) + (P[2][I] * P[0][J])
						+ 2.0f * (P[2][I] * P[2][J]) + (P[0][I] * P[1][J]) + (P[1][I] * P[0][J]);

	return result;
}

struct tri {
	glm::f64vec3 P[3];
};

void PhysObject::compute_inertia_tensor(glm::float64 density, glm::f64mat3* I0_out, glm::f64vec3* CoM_out, glm::float64* M_out, glm::float64* Vol_out)
{
	Mesh* mesh = model.getMesh(0);
	
	glm::float64 V = 0.0;
	glm::float64 mass = 0.0;
	glm::f64vec3 MassCenter = glm::f64vec3(0.0, 0.0, 0.0);
	//glm::vec3 MassCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::float64 Ia = 0.0, Ib = 0.0, Ic = 0.0, Iap = 0.0, Ibp = 0.0, Icp = 0.0;

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
		glm::f64vec3 P[3];

		P[0] = faces[i].P[2];
		P[1] = faces[i].P[1];
		P[2] = faces[i].P[0];

		//for (int j = 0; j < 3; j++)
		//{
		//	glm::vec3 localP;

		//}

		glm::float64 detJ = glm::dot(P[0], glm::cross(P[1], P[2]));
		glm::float64 tetraVol = detJ / 6.0;
		glm::float64 tetraMass = density * tetraVol;
		glm::f64vec3 tetraCoM = (P[0] + P[1] + P[2]) / 4.0;

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
	Ia = Ia * density / 60.0 - mass * (MassCenter.y * MassCenter.y + MassCenter.z * MassCenter.z);
	Ib = Ib * density / 60.0 - mass * (MassCenter.x * MassCenter.x + MassCenter.z * MassCenter.z);
	Ic = Ic * density / 60.0 - mass * (MassCenter.x * MassCenter.x + MassCenter.y * MassCenter.y);

	Iap = Iap * density / 120.0 - mass * MassCenter.y * MassCenter.z;
	Ibp = Ibp * density / 120.0 - mass * MassCenter.x * MassCenter.y;
	Icp = Icp * density / 120.0 - mass * MassCenter.x * MassCenter.z;

	glm::mat3 I = glm::mat3(Ia, -Iap, -Ibp,
							-Iap, Ib, -Icp,
							-Ibp, -Icp, Ic);

	*I0_out = I;
	*CoM_out = MassCenter;
	*M_out = mass;
	*Vol_out = V;
}
