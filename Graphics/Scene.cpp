#include "Scene.h"


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
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);

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
		models[i].Draw(shaderProgram, camera);
	}


	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	// Take care of all GLFW events
	glfwPollEvents();
}

Model* Scene::addModel(const char* modelPath)
{
	Model model = Model(modelPath);
	models.push_back(model);
	return &models[models.size() - 1];
}