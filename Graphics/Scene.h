#pragma once
#include "Model.h"
#include "Light.h"


class Scene
{
public:
	Scene(int width, int height, const char* title);
	~Scene();
	void Draw();

	//Model addModel(const char* modelPath);
	void addModel(Model* model);
	GLFWwindow* getWindow() { return window; }

	Camera* getCamera() { return &camera; }
	Light* getLight() { return &light; }
	Shader* getShader() { return &shaderProgram; }



private:
	int width, height;

	Model* floor_model;

	GLFWwindow* window;

	Shader shaderProgram;
	Camera camera;
	Light light;

	//list of model objects
	std::vector<Model*> models;

	// Variables to create periodic event for FPS displaying
	double prevTime;
	double crntTime;
	double timeDiff;

	unsigned int counter;

	const char* title;

};