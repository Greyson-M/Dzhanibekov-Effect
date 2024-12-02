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
	
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 angularVelocity;
	glm::vec3 angularAcceleration;

};