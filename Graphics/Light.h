#pragma once

#include "shaderClass.h"
#include "Camera.h"

class Light
{
public:
	Light(Shader* shader_prog);
	Light(Shader* shader_prog, glm::vec3 position, glm::vec4 color);
	Light();
	~Light();

	void setPosition(glm::vec3 pos) { position = pos; }
	glm::vec3 getPosition() { return position; }

	void update();

private:
	glm::vec3 position;
	glm::vec4 color;
	float intensity;
	Shader* shaderProgram;

	glm::mat4 model_mat;

};