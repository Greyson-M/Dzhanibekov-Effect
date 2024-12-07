#include "Light.h"

Light::Light(Shader* shader_prog)
{
	shaderProgram = shader_prog;

	// Take care of all the light related things
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.5f, 0.5f, 0.5f);
	model_mat = glm::mat4(1.0f);
	model_mat = glm::translate(model_mat, position);

	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), color.x, color.y, color.z, color.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), position.x, position.y, position.z);

}

Light::Light(Shader* shader_prog, glm::vec3 position, glm::vec4 color)
{
	shaderProgram = shader_prog;

	// Take care of all the light related things
	this->color = color;
	this->position = position;
	model_mat = glm::mat4(1.0f);
	model_mat = glm::translate(model_mat, position);

	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), color.x, color.y, color.z, color.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), position.x, position.y, position.z);

}



Light::Light()
{
	std::cerr << "Light object created without shader program" << std::endl;
}

Light::~Light()
{
	std::cerr << "Light object deleted" << std::endl;
}

void Light::update()
{
	model_mat = glm::mat4(1.0f);
	model_mat = glm::translate(model_mat, position);
	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), color.x, color.y, color.z, color.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), position.x, position.y, position.z);
}
