#include "Environment.h"

Environment::Environment(int width, int height, const char* title)
    : scene(width, height, title) // Initialize scene using member initializer list
{
    physObjects = std::vector<PhysObject>();
}

//Environment::~Environment()
//{
//    for (auto& physObject : physObjects)
//    {
//        // Assuming PhysObject has a destructor that handles its own cleanup
//    }
//    physObjects.clear();
//}

PhysObject* Environment::addPhysObject(const char* model_path)
{
    Model model = Model(model_path);
    PhysObject phys_obj(model);
    physObjects.push_back(phys_obj);

	PhysObject* return_ref = &physObjects[physObjects.size() - 1];
    Model* model_ref = return_ref->get_model_ptr();

	scene.addModel(model_ref);

    return &(physObjects[physObjects.size() - 1]);
}

void Environment::update()
{
	for (int j = 0; j < Settings().sim_freq; j++)
	{
		for (int i = 0; i < physObjects.size(); i++)
		{
			physObjects[i].update();
		}
	}
}

void Environment::draw()
{
    scene.Draw();
}
