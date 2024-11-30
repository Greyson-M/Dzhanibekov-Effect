#include "Environment.h"

Environment::Environment(int width, int height, const char* title)
{

	std::vector<void*> physObjects;
	floorHeight = 0.0f;
	dt = 0.01f;

	Scene scene(width, height, title);


}
