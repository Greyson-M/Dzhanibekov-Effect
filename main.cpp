#include "Graphics/Scene.h"

const unsigned int width = 800;
const unsigned int height = 800;


int main()
{
	const char* modelPath = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/Models/TOP/scene.gltf";

	Scene scene(width, height, "test");

	Model* bunny = scene.addModel(modelPath);

	//rotate bunny
	//bunny->setOrientation(glm::quat(0.0f, 0.0f, 1.0f, 0.0f));
	//bunny->setScale(glm::vec3(100.0f, 100.0f, 100.0f));
	//bunny->setTranslation(glm::vec3(100.0f, 0.0f, 0.0f));


	// Main while loop
	while (!glfwWindowShouldClose(scene.getWindow()))
	{
		scene.Draw();
	}


	return 0;
}