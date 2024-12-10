#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Graphics/Scene.h"
#include "PhysObject.h"

class Environment
{
public:
	Environment(int width, int height, const char* title);
	//~Environment();

	PhysObject* addPhysObject(const char* model_path);
	PhysObject* getPhysObject(int index) { return &physObjects[index]; }

	void update();
	void draw();

	void step() { update(); draw(); };

	void write_data(std::string str);
	void close_stream();

	Scene* getScene() { return &scene; }

private:
	//vector of physobjects
	std::vector<PhysObject> physObjects;
	Scene scene;

	std::ofstream data_record;

};

#endif