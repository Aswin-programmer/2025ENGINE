#include <iostream>
#include <cstring>
#include <vector>

#include "SCRIPTING/NativeCPP/NativeCPPGlobalScript.h"

class MyScript : public NativeCPPGlobalScript
{
public:
	void OnIntialize() override
	{
		std::cout << "The OnInitialize function function is working!." << std::endl;
		// Creating a new entity for testing.

		std::cout << "[SCRIPT] &g_World: " << ecsWorld.get() << "\n";

		flecs::entity e2 = ecsWorld->FindEntityByName("Test2");
		std::cout<<e2.has<TransfromComponent>()<<std::endl;
		if (e2.is_valid())
		{
			std::cout<<"Entered the test!.\n";
			TransfromComponent& transform = e2.get_mut<TransfromComponent>();
			transform.Position.x = 10;
		}

		for(int i=0;i<v1.size();i++)
		{
			v1[i] = 5;
		}

		flecs::entity e1 = ecsWorld->FindEntityByName("Test1");
		std::cout<<e1.has<TransfromComponent>()<<std::endl;

		static bool enter = true;
		if (enter == true)
		{
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					flecs::entity e = ecsWorld->CreateEntity("Temp" + std::to_string(i) + "_" + std::to_string(j));
					e
						.set<TransfromComponent>({ glm::vec3(i, j, 0.f)
						, glm::vec3(0.f, 0.f, 0.f), glm::vec3(10.f, 10.f, 10.f) })
						.set<MeshComponent>({ "Avocado.gltf" });
				}
			}
			enter = false;
		}
	}

	void OnUpdate() override
	{
		std::cout << "The OnUpdate function is function is working!." << std::endl;
	}

	void OnShutDown() override
	{
		std::cout << "The OnShutdown function is actually working!." << std::endl;
	}

private:

};

#ifdef _WIN32
	#define EXPORT_API __declspec(dllexport)
#else
	#define EXPORT_API
#endif

extern "C" EXPORT_API NativeCPPGlobalScript* CreateScript()
{
	return new MyScript();
}

extern "C" EXPORT_API void DestroyScript(NativeCPPGlobalScript* s)
{
	delete s;
}
