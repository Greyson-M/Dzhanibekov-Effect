#include "PhysObject.h"

PhysObject::PhysObject(Model model) : model(model)
{
	mass = 1.0f;

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

PhysObject::PhysObject(const char* model_path) : model(model_path)
{
	mass = 1.0f;

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	angularVelocity = glm::vec3(1.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

PhysObject::~PhysObject()
{
}

void PhysObject::update()
{
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity += acceleration * Settings().dt;
	model.setTranslation(model.getTranslation() + velocity * Settings().dt);

	if (model.getTranslation().y < Settings().floor_height)
	{
		model.setTranslation(glm::vec3(model.getTranslation().x, Settings().floor_height, model.getTranslation().z));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//time for inertia tensor
	//then eulers equations of rigid body motion

}
