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
	topobj->setMomentum(glm::f64vec3(25.0, 0.0, -1.0));

	PhysObject* topobj1 = env->addPhysObject(modelPath);
	topobj1->translate(glm::vec3(0.0f, 1.0f, 0.0f));
	topobj1->rotate(glm::quat(0.0f, 1.0f, 0.0f, 1.0f));
	topobj1->setMomentum(glm::f64vec3(25.0, 0.0, 1.0));

	//PhysObject* topobj2 = env->addPhysObject(modelPath2);
	//topobj2->translate(glm::vec3(1.0f, 0.0f, 0.0f));


	Scene* scene = env->getScene();
	GLFWwindow* window = scene->getWindow();

	while (!glfwWindowShouldClose(window))
	{
		env->step();
	}


	glm::f64vec3 L0 = env->getPhysObject(0)->getMomentum();
	glm::f64vec3 L1 = env->getPhysObject(1)->getMomentum();
	
	//std::string L0_tex = "$L_0 = [" + std::to_string(L0.x) + "\\quad" + std::to_string(L0.y) + "\\quad" + std::to_string(L0.z) + "]$";
	//std::string L1_tex = "$L_1 = [" + std::to_string(L1.x) + "\\quad" + std::to_string(L1.y) + "\\quad" + std::to_string(L1.z) + "]$";

// roound to 2 decimal places

	float l0x = round(L0.x * 100) / 100;
	float l0y = round(L0.y * 100) / 100;
	float l0z = round(L0.z * 100) / 100;

	float l1x = round(L1.x * 100) / 100;
	float l1y = round(L1.y * 100) / 100;
	float l1z = round(L1.z * 100) / 100;


	std::string L0_tex = "$L_0 = [" + std::to_string(l0x).substr(0, 5) + "\\quad" + std::to_string(l0y).substr(0, 5) + "\\quad" + std::to_string(l0z).substr(0, 5) + "]$";
	std::string L1_tex = "$L_1 = [" + std::to_string(l1x).substr(0, 5) + "\\quad" + std::to_string(l1y).substr(0, 5) + "\\quad" + std::to_string(l1z).substr(0, 5) + "]$";

	env->write_data("L0: " + L0_tex);
	env->write_data("L1: " + L1_tex);
	env->close_stream();



	return 0;
}