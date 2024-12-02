#pragma once
#include <glm/glm.hpp>

struct Settings
{
	// Window settings
	int width = 800;
	int height = 800;
	const char* title = "test";

	// Physics settings
	glm::float32 floor_height = 0.0f;
	glm::float32 dt = 0.01f;

};