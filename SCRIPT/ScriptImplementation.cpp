#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include <array>
#include <string>

#include <glad/glad.h>

#include <INPUT/KeyBoard.h>
#include <INPUT/Mouse.h>

#include "SCRIPTING/NativeCPP/NativeCPPGlobalScript.h"
#include "REACTPHYSICS3D/ReactPhysics3DCore.h"
#include "ECS/COMPONENTS/PhysicsComponent.h"
#include "ECS/COMPONENTS/IdentiferComponent.h"
#include "SERIALIZATION/Serialization.h"
#include "GlobalInformation/GlobalInformation.h"
#include "Random.h"
#include "Timer.h"

enum class GameState{
    UI = 0,
    GAMEPLAY = 1
};

 static bool firstTime = true;
 GameState gameState;
 static bool g_TriggerGameOver = false;

class MyCollisionListener : public rp3d::EventListener
{
    public:
        MyCollisionListener(){}
        MyCollisionListener(std::shared_ptr<ECSWorld> ecsWorld)
            :
            ecsWorld{ecsWorld}
        {

        }
        void onContact(const rp3d::CollisionCallback::CallbackData& callbackData) override
        {
            std::shared_ptr<flecs::world> world = ecsWorld->GetWorld();
            for(uint32_t i=0;i<callbackData.getNbContactPairs();i++)
            {
                
                rp3d::CollisionCallback::ContactPair contactPair = 
                callbackData.getContactPair(i);

                rp3d::RigidBody* body1 = static_cast<rp3d::RigidBody*>(contactPair.getBody1());
                rp3d::RigidBody* body2 = static_cast<rp3d::RigidBody*>(contactPair.getBody2());
                uint64_t id1 = reinterpret_cast<uint64_t>(body1->getUserData());
                
                uint64_t id2 = reinterpret_cast<uint64_t>(body2->getUserData());

                flecs::entity e1 = world->entity(id1);
                flecs::entity e2 = world->entity(id2);

                if (e1.is_alive() && e2.is_alive())
                {
                    // FAST CHECK: Use names directly without allocating new strings, 
                    // or better yet, use Tags like e1.has<PlayerTag>()
                    const char* n1 = e1.name().c_str();
                    const char* n2 = e2.name().c_str();
                    
                    // Simple strstr check is faster than creating std::string objects
                    bool pCollidesEnemy = (strcmp(n1, "p") == 0 && strstr(n2, "enemy") != nullptr);
                    bool enemyCollidesP = (strstr(n1, "enemy") != nullptr && strcmp(n2, "p") == 0);

                    if(pCollidesEnemy || enemyCollidesP)
                    {
                        // Just flag a component or set a boolean. 
                        // DO NOT std::cout here in production code.
                        firstTime = true;
                        gameState = GameState::UI;
                        g_TriggerGameOver = true;
                    }

                }
            }
        }
    private:
        std::shared_ptr<ECSWorld> ecsWorld;
};


class MyScript : public NativeCPPGlobalScript
{
public:
    MyScript(){}
	void OnIntialize() override
	{
		std::cout << "The OnIni tialize ffunction is working!." << std::endl;
		// Creating a new entity for testing.
		 
		std::cout << "[SCRIPT] &g_World: " << ecsWorld.get() << "\n"; 
		static bool enter = true;
		if (enter == true)
		{

			enter = false;
		}

        listener = MyCollisionListener(ecsWorld);
        physicsCore->GetPhysicsWorld()->setEventListener(&listener);

        gameState = GameState::UI;

        Random::Init();

        timer = Timer();

        GlobalInformation::IsScenePlaying = true;
	}

	void OnUpdate() override
    {
        if(g_TriggerGameOver == true)
        {
            enemyEntities.clear();
            ecsWorld->GetWorld()->defer_begin();

            ecsWorld->GetWorld()->each<IdentiferComponent>([&](flecs::entity e, IdentiferComponent& transfromC) {
            if (e.name() != "Test7")
            {
                e.destruct();
            }
            });
            ecsWorld->GetWorld()->defer_end();
            g_TriggerGameOver = false;
            std::cout << "World Cleared!." << std::endl;
        }
        OliUIBegin(oliUIMainContext);
        if(gameState == GameState::UI)   
        {
            std::cout<<"UI STATE"<<std::endl;   

            OliUIBeginWindowContainer(oliUIMainContext, OliUIRect{160, 120, 320, 240});
            OliUIBeginLayout(oliUIMainContext, std::array<int, 5>{40, 60, 40, 60, 40}.data(), 5, OliUILayoutVertical);
            
            OliUIBeginSubWindowContainer(oliUIMainContext, "temp1", sizeof("temp1"));
            OliUIEndSubWindowContainer(oliUIMainContext);

            OliUIMouseClickState state1 = OliUIBeginSubWindowContainer(oliUIMainContext, "first", sizeof("first"));
            if(state1 & OliUIJustClicked){
                gameState = GameState::GAMEPLAY;
                std::cout<<"The Button Is Clicked-1."<<std::endl;  
            }
            OliUIDrawTexturedRectangle(oliUIMainContext, OliUITexture{"START"});
            OliUIEndSubWindowContainer(oliUIMainContext);

            OliUIBeginSubWindowContainer(oliUIMainContext, "temp2", sizeof("temp2"));
            OliUIEndSubWindowContainer(oliUIMainContext);

            OliUIMouseClickState state2 = OliUIBeginSubWindowContainer(oliUIMainContext, "second", sizeof("second"));
            if (state2 & OliUIJustClicked) {
                std::cout << "The Button Is Clicked-2." << std::endl;
            }
            OliUIDrawTexturedRectangle(oliUIMainContext, OliUITexture{ "CONTINUE" });
            OliUIEndSubWindowContainer(oliUIMainContext);

            OliUIBeginSubWindowContainer(oliUIMainContext, "temp3", sizeof("temp3"));
            OliUIEndSubWindowContainer(oliUIMainContext);

            OliUIEndLayout(oliUIMainContext);
            OliUIEndWindowContainer(oliUIMainContext); 
        }
        else if(gameState == GameState::GAMEPLAY)
        {
            if(firstTime){
                Serialization::LoadScene(
                    Serialization::serializationPath + "data.bin",
                    SerializationMode::BINARY,
                    ecsWorld->GetWorld()
                );
                std::cout << "Data Loaded from data.bin" << std::endl;
                firstTime = false;
            }

            flecs::entity p = ecsWorld->GetWorld()->entity("p");
            if(!p.is_alive() || !p.has<PhysicsComponent>()) return;

            PhysicsComponent& physicsP = p.get_mut<PhysicsComponent>();
            if(!physicsP.rigidBody) return;

            TransfromComponent& transfromP = p.get_mut<TransfromComponent>();
            //std::cout<<"x : "<<transfromP.GetPosition().x<<", y : "<<transfromP.GetPosition().y<<", z : "<<transfromP.GetPosition().z<<std::endl;

            rp3d::Vector3 moveDirP(0, 0, 0);
            if (Keyboard::IsKeyPressed(KEY_A)) moveDirP.x -= 1.0f;
            if (Keyboard::IsKeyPressed(KEY_D)) moveDirP.x += 1.0f;
            if (Keyboard::IsKeyPressed(KEY_W)) moveDirP.z -= 1.0f;
            if (Keyboard::IsKeyPressed(KEY_S)) moveDirP.z += 1.0f;
            if (moveDirP.lengthSquare() > 0.0f)
            {
                moveDirP.normalize();
            }
            rp3d::RigidBody* bodyP = physicsP.rigidBody;
            bodyP->setLinearVelocity(
                moveDirP
            );

            // Creating the enemies:
            static bool createEnemies = false;
            static int enemyIndex = 0;

            if(timer.ElapsedSeconds() > 5){
                createEnemies = true;
                std::cout<<"Enemy Spanned" <<std::endl;
                timer.Reset();
            }

            if(createEnemies){
                ecsWorld->GetWorld()->defer_begin();
                std::string enemyName = std::string("enemy" + std::to_string(enemyIndex++));
                flecs::entity enemy = ecsWorld->CreateEntity(enemyName);
                enemy
                    .set<IdentiferComponent>({enemyName})
                    .set<TransfromComponent>({
                        glm::vec3(Random::GetRandomFloat(-2.90, 2.72), 1.99, -33.05),
                        glm::vec3(0.f),
                        glm::vec3(1.f)
                    }) 
                    .set<MeshComponent>({"enemyBlock.gltf", 1, 1, 1})
                    .set<PhysicsComponent>({
                        PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER,
                        PHYSICSBODYTYPE::KINEMATICMESH,
                        true
                    });
                enemyEntities.push_back(enemy);
                ecsWorld->GetWorld()->defer_end();
                createEnemies = false;
            }
            auto it = enemyEntities.begin();
            while(it != enemyEntities.end())
            {
                if(it->has<TransfromComponent>()){
                    TransfromComponent& transfromEnemy = it->get_mut<TransfromComponent>();
                    if(transfromEnemy.GetPosition().z > 33){
                        it->destruct();
                        if(!it->is_alive())
                        {
                            it = enemyEntities.erase(it);
                        }
                    }
                    else
                    {
                        PhysicsComponent& physicsEnemy = it->get_mut<PhysicsComponent>();
                        if(!physicsEnemy.rigidBody) continue;

                        rp3d::RigidBody* bodyEnemy = physicsEnemy.rigidBody;
                        bodyEnemy->setLinearVelocity(
                            rp3d::Vector3(0.f, 0.f, 1.f)
                        );
                        ++it;
                    }
                }
            }
        }  

        OliUIEnd(oliUIMainContext);
    }


	void OnShutDown() override                                  
	{
		// auto world = ecsWorld->GetWorld();

		// world->defer_begin();  // start deferring table modifications

		// world->query<TransfromComponent>().each([](flecs::entity e, TransfromComponent& p) {
		// 	if(e.name() == flecs::string_view("Test7")){
		// 		return;
		// 	}
		// 	e.destruct();  // this is deferred safely  
		// });

		// world->defer_end(); // apply all deferred operations
		std::cout << "The OnShutdown function is actually working!." << std::endl;
	}  

private:
    MyCollisionListener listener;
    Timer timer;
    std::vector<flecs::entity> enemyEntities;
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