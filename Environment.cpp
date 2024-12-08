#include "Environment.h"

Environment::Environment(int width, int height, const char* title)
    : scene(width, height, title) // Initialize scene using member initializer list
{
    physObjects = std::vector<PhysObject>();

	data_record.open("data.txt");

	////set labels
	//std::string L0_tex = "$L0 = [25 \\quad 0 \\quad -1]$";
	//std::string L1_tex = "$L1 = [25 \\quad 0 \\quad 1]$";
	//data_record << "L0: " << L0_tex << std::endl;
	//data_record << "L1: " << L1_tex << std::endl;

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

	for (int i = 0; i < physObjects.size(); i++) {
		glm::f64vec3 omega = physObjects[i].get_omega();

		data_record << "OMEGA" << i << ": " << omega.x << "," << omega.y << "," << omega.z << std::endl;
	}

}

void Environment::draw()
{
    scene.Draw();
}

void Environment::write_data(std::string str)
{
	data_record << str << std::endl;
}

void Environment::close_stream()
{
	data_record.close();
}
