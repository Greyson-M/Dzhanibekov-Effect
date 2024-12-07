#pragma once

#include "Graphics/Model.h"
#include "settings.h"


class PhysObject
{
public:
	PhysObject(Model model);
	PhysObject(const char* model_path);

	~PhysObject();

	void update();

	Model* get_model_ptr() { return &model; }

	void translate(glm::vec3 translation) { model.setTranslation(translation); }
	void rotate(glm::quat rotation) { model.setOrientation(rotation); }
	void scale(glm::vec3 scale) { model.setScale(scale); }

private:
	Model model;

	glm::float32 mass;
	glm::vec3 com;

	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 omega;
	glm::vec3 angularAcceleration;

	glm::vec3 L;	//angular momentum

	glm::mat3 I;
	glm::mat3 I_inv;

	void compute_inertia_tensor(glm::float32 density, glm::mat3* I0, glm::vec3* com, glm::float32* mass, glm::float32* Vol_out );
	glm::float32 ComputeInertiaMoment(glm::vec3* P, int I);
	glm::float32 ComputeInertiaProduct(glm::vec3* P, int I, int J);

};