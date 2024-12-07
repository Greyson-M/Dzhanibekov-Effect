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
	glm::float64 dt = 0.0001f;
	int sim_freq = 1000;

	const char* resources_path = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/";

};