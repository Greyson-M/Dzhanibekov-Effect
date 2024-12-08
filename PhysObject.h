#pragma once

#include "Graphics/Model.h"
#include "settings.h"
#include <iostream>
#include <fstream>


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

	glm::f64vec3 get_omega();
	void setMomentum(glm::f64vec3 L) { this->L = L; }
	glm::f64vec3 getMomentum() { return L; }

private:
	Model model;

	glm::float64 mass;
	glm::f64vec3 com;

	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::f64vec3 omega;
	glm::f64vec3 angularAcceleration;

	glm::f64vec3 L;	//angular momentum

	glm::f64mat3 I;
	glm::f64mat3 I_inv;

	void compute_inertia_tensor(glm::float64 density, glm::f64mat3* I0, glm::f64vec3* com, glm::float64* mass, glm::float64* Vol_out );
	glm::float64 ComputeInertiaMoment(glm::f64vec3* P, int I);
	glm::float64 ComputeInertiaProduct(glm::f64vec3* P, int I, int J);

};