#include <iostream>
#include <cstring>
#include <vector>

#include "SCRIPTING/NativeCPP/NativeCPPGlobalScript.h"

class MyScript : public NativeCPPGlobalScript
{
public:
	void OnIntialize() override
	{
		std::cout << "The OnInitialize ffunction is working!." << std::endl;
		// Creating a new entity for testing.
		 
		std::cout << "[SCRIPT] &g_World: " << ecsWorld.get() << "\n"; 

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
		std::cout<<"Hell ow"<<std::endl;  

		// Print once per frame for debugging (optional)
    std::cout << "The OnUpdate function is working!." << std::endl;

    // //Rotate all entities with TransfromComponent
    // float rotationSpeed = 1.0f; // degrees per frame (you can adjust)
    
    // ecsWorld->GetWorld()->query<TransfromComponent>().each([rotationSpeed](flecs::entity e, TransfromComponent& t) {
    //    // Increment rotation around Y-axis
    //    t.Rotation.y += rotationSpeed;

    //    if (t.Rotation.y >= 360.f)
    //        t.Rotation.y -= 360.f;


    //    t.Rotation.x += rotationSpeed * 0.5f;
    //    t.Rotation.z += rotationSpeed * 0.25f;
    // });
	}

	void OnShutDown() override
	{
		auto world = ecsWorld->GetWorld();

		world->defer_begin();  // start deferring table modifications

		world->query<TransfromComponent>().each([](flecs::entity e, TransfromComponent& p) {
			e.destruct();  // this is deferred safely
		});

		world->defer_end(); // apply all deferred operations
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
