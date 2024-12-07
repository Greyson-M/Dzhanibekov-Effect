#include "Scene.h"
#include "../settings.h"

Scene::Scene(int width, int height, const char* title)
{	
	this->width = width;
	this->height = height;
	this->title = title;

	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	shaderProgram = Shader("E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Graphics/default.vert",
		"E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Graphics/default.frag");

	light = Light(&shaderProgram);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	// Creates camera object
	camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Variables to create periodic event for FPS displaying
	this->prevTime = 0.0;
	this->crntTime = 0.0;
	this->timeDiff;
	// Keeps track of the amount of frames in timeDiff
	this->counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);

	const char* floor_path = "E:/Programming/physics/DzhanibekovEffect/c++vs/Renderer/Resources/Models/floor/scene.gltf";
	floor_model = new Model(floor_path);
	addModel(floor_model);

	std::string light_path = Settings().resources_path + std::string("Models/cube/scene.gltf");
	const char* light_path_c = light_path.c_str();
	light_model = new Model(light_path_c);
	addModel(light_model);

	//translate light model to light position
	glm::vec3 model_pos = light_model->getTranslation();
	glm::vec3 light_pos = light.getPosition();
	light_model->setTranslation(model_pos - light_pos);

}

Scene::~Scene()
{	
	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	std::cout << "Scene object deleted" << std::endl;
}

void Scene::Draw()
{	
	//check inputs
	Inputs();

	// Updates counter and times
	crntTime = glfwGetTime();
	timeDiff = crntTime - prevTime;
	counter++;

	if (timeDiff >= 1.0 / 30.0)
	{
		// Creates new title
		std::string FPS = std::to_string((1.0 / timeDiff) * counter);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string newTitle = title + FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(window, newTitle.c_str());

		// Resets times and counter
		prevTime = crntTime;
		counter = 0;

		// Use this if you have disabled VSync
		//camera.Inputs(window);
	}

	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Handles camera inputs (delete this if you have disabled VSync)
	camera.Inputs(window);
	// Updates and exports the camera matrix to the Vertex Shader
	camera.updateMatrix(45.0f, 0.1f, 100.0f);


	// Draw the normal model
	/*model.Draw(shaderProgram, camera);*/
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Draw_model(shaderProgram, camera);
	}


	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	// Take care of all GLFW events
	glfwPollEvents();
}

void Scene::addModel(Model* model)
{
	models.push_back(model);
}

void Scene::Inputs()
{// Check if the ESC key had been pressed or if the window had been closed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window))
		glfwSetWindowShouldClose(window, true);
	{
	}

	float speed = 0.05f;

	//move light position with arrow keys
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(0.0f, 0.0f, speed));
		light.update();

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(0.0f, 0.0f, -speed));
		light.update();

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(-speed, 0.0f, 0.0f));
		light.update();

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(speed, 0.0f, 0.0f));
		light.update();

	}

	//q and e to move light up and down
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(0.0f, speed, 0.0f));
		light.update();

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		light.setPosition(light.getPosition() + glm::vec3(0.0f, -speed, 0.0f));
		light.update();

	}
	
	light_model->setTranslation(light.getPosition());

	////translate model position to light position
	//glm::vec3 model_pos = light_model->getTranslation();
	//glm::vec3 light_pos = light.getPosition();
	//light_model->setTranslation(light_pos - model_pos);
	//	
}



//
//Model Scene::addModel(const char* modelPath)
//{
//	Model model = Model(modelPath);
//	models.push_back(&model);
//	return model;
//}