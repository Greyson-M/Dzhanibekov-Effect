#pragma once

#include "Graphics/Model.h"
#include "Environment.h"

class PhysObject
{
public:
	PhysObject(const char* model_path, Environment* env);
	void update();

private:
	Model model;
	Environment* environment;

	Camera* camera;
	Shader* shader;

	glm::float32 mass;
	
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 angularVelocity;
	glm::vec3 angularAcceleration;

};