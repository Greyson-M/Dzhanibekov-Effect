#pragma once

#include "Graphics/Scene.h"
#include "PhysObject.h"

class Environment
{
public:
	Environment(int width, int height, const char* title);
	//~Environment();

	PhysObject* addPhysObject(const char* model_path);

	void update();
	void draw();

	void step() { update(); draw(); };

	Scene* getScene() { return &scene; }

private:
	//vector of physobjects
	std::vector<PhysObject> physObjects;

	Scene scene;
};