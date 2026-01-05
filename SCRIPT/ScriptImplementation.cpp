#include <iostream>
#include <cstring>
#include <vector>
#include <random>

#include <glad/glad.h>

#include <INPUT/KeyBoard.h>
#include <INPUT/Mouse.h>

#include "SCRIPTING/NativeCPP/NativeCPPGlobalScript.h"
#include "REACTPHYSICS3D/ReactPhysics3DCore.h"
#include "ECS/COMPONENTS/PhysicsComponent.h"


class MyScript : public NativeCPPGlobalScript
{
public:
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
	}

	void OnUpdate() override
    {
        flecs::entity e = ecsWorld->GetWorld()->entity("b3");
        if (!e.is_alive() || !e.has<PhysicsComponent>()) return;

        PhysicsComponent& phys = e.get_mut<PhysicsComponent>();
        if (!phys.rigidBody) return;

        rp3d::RigidBody* body = phys.rigidBody;

        rp3d::Vector3 velocity = body->getLinearVelocity();

        // ------------------------------------------------
        // Ground check (simple but stable)
        // ------------------------------------------------
        bool isGrounded = std::abs(velocity.y) < 0.05f;

        // ------------------------------------------------
        // Input
        // ------------------------------------------------
        rp3d::Vector3 moveDir(0, 0, 0);

        if (Keyboard::IsKeyPressed(KEY_A)) moveDir.x -= 1.0f;
        if (Keyboard::IsKeyPressed(KEY_D)) moveDir.x += 1.0f;
        if (Keyboard::IsKeyPressed(KEY_W)) moveDir.z -= 1.0f;
        if (Keyboard::IsKeyPressed(KEY_S)) moveDir.z += 1.0f;

        if (moveDir.lengthSquare() > 0.0f)
            moveDir.normalize();

        float control = isGrounded ? 1.0f : AIR_CONTROL;

        // ------------------------------------------------
        // Movement force
        // ------------------------------------------------
        body->applyWorldForceAtCenterOfMass(
            moveDir * MOVE_FORCE * control
        );

        // ------------------------------------------------
        // Jump (impulse)
        // ------------------------------------------------
        if (Keyboard::IsKeyJustPressed(KEY_SPACE) && isGrounded)
		{
			rp3d::Vector3 vel = body->getLinearVelocity();
			vel.y = JUMP_VELOCITY;   // instant upward velocity
			body->setLinearVelocity(vel);
		}


        // ------------------------------------------------
        // Speed clamp (horizontal only)
        // ------------------------------------------------
        rp3d::Vector3 horizVel(velocity.x, 0, velocity.z);
        float speed = horizVel.length();

        if (speed > MAX_SPEED)
        {
            rp3d::Vector3 capped = horizVel.getUnit() * MAX_SPEED;
            body->setLinearVelocity(
                rp3d::Vector3(capped.x, velocity.y, capped.z)
            );
        }

        // ------------------------------------------------
        // Manual friction
        // ------------------------------------------------
        if (isGrounded && moveDir.lengthSquare() == 0.0f)
        {
            body->applyWorldForceAtCenterOfMass(
                rp3d::Vector3(-velocity.x, 0, -velocity.z) * DAMPING_FORCE
            );
        }

		flecs::entity e1 = ecsWorld->GetWorld()->entity("b2");
        if (!e.is_alive() || !e.has<PhysicsComponent>()) return;

		PhysicsComponent& physicsb3 = e1.get_mut<PhysicsComponent>();
		rp3d::RigidBody* bodyB3 = physicsb3.rigidBody;
		rp3d::Vector3 posB3 = bodyB3->getTransform().getPosition();
		static bool isRight = false;
		if(posB3.x > 5.0f){
			isRight = false;
		}
		else if(posB3.x < -5.0f){
			isRight = true;
		}
		if(isRight){
			bodyB3->setLinearVelocity(rp3d::Vector3(2.0f, 0.0f, 0.0f));
		}
		else{
			bodyB3->setLinearVelocity(rp3d::Vector3(-2.0f, 0.0f, 0.0f));
		}
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
	static constexpr float MOVE_FORCE    = 60.0f;
    static constexpr float AIR_CONTROL   = 0.35f;
    static constexpr float MAX_SPEED     = 6.0f;
    static constexpr float JUMP_IMPULSE  = 6.5f;
    static constexpr float DAMPING_FORCE = 8.0f;
	static constexpr float JUMP_VELOCITY = 10.f;
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
