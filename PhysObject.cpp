#include "PhysObject.h"

PhysObject::PhysObject(const char* model_path, Environment* env)
{
	model = Model(model_path);
	environment = env;

	camera = environment->getScene()->getCamera();
	shader = environment->getScene()->getShader();
}

void PhysObject::update()
{
	acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	velocity += acceleration * environment->dt;
	model.setTranslation(model.getTranslation() + velocity * environment->dt);

	if (model.getTranslation().y < environment->floorHeight)
	{
		model.setTranslation(glm::vec3(model.getTranslation().x, environment->floorHeight, model.getTranslation().z));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	model.Draw(*shader, *camera);

}
