#include "Environment.h"
#include "Settings.h"

int main()
{
	//const char* modelPath = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/Models/tinytoplong/scene.gltf";
	//const char* modelPath = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/models/TOP/scene.gltf";
	const char* modelPath = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/models/test/scene.gltf";

	//Scene scene(width, height, "test");
	//Model* bunny = scene.addModel(modelPath);

	Environment* env = new Environment(Settings().width, Settings().height, "test");
	//rotate bunny
	//bunny->setOrientation(glm::quat(0.0f, 0.0f, 1.0f, 0.0f));
	//bunny->setScale(glm::vec3(100.0f, 100.0f, 100.0f));
	//bunny->setTranslation(glm::vec3(100.0f, 0.0f, 0.0f));

	PhysObject* topobj = env->addPhysObject(modelPath);
	topobj->translate(glm::vec3(0.0f, 1.0f, 0.0f));
	topobj->rotate(glm::quat(0.0f, 1.0f, 0.0f, 1.0f));

	//PhysObject* topobj2 = env->addPhysObject(modelPath2);
	//topobj2->translate(glm::vec3(1.0f, 0.0f, 0.0f));


	Scene* scene = env->getScene();
	GLFWwindow* window = scene->getWindow();

	while (!glfwWindowShouldClose(window))
	{
		env->step();
	}


	return 0;
}