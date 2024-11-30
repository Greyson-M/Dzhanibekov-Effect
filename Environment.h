#pragma once

#include "Graphics/Scene.h"

class Environment
{
public:
	Environment(int width, int height, const char* title);
	~Environment();

	void addPhysObject(void* physObject);

	glm::float32 floorHeight;
	glm::float32 dt;

	Scene* getScene() { return &scene; }

private:
	//vector of physobjects
	std::vector<void*> physObjects;

	Scene scene;
	

};