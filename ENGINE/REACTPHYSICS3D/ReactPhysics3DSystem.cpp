#include "ReactPhysics3DSystem.h"

bool ReactPhysics3DSystem::IsDebugEnabled = GlobalInformation::IsReactPhysics3DDebuggerEnabled;

ReactPhysics3DSystem::ReactPhysics3DSystem()
{
}

ReactPhysics3DSystem::ReactPhysics3DSystem(
	std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore, 
	std::shared_ptr<flecs::world> ecsWorld
)
	:
	reactPhysics3DCore{reactPhysics3DCore},
	ecsWorld{ecsWorld}
{
	meshPhysicsQuery = ecsWorld->query_builder<TransfromComponent, MeshComponent, PhysicsComponent>()
		.build();
}


ReactPhysics3DSystem::~ReactPhysics3DSystem()
{
}

void ReactPhysics3DSystem::CommitReactPhysics3DState()
{
    //Destroy existing rigid bodies
    auto* world = reactPhysics3DCore->GetPhysicsWorld();
    auto physicsCommon = reactPhysics3DCore->GetPhysicsCommon();

    std::vector<rp3d::RigidBody*> bodies;
    bodies.reserve(world->getNbRigidBodies());

    for (rp3d::uint32 i = 0; i < world->getNbRigidBodies(); ++i)
    {
        bodies.push_back(world->getRigidBody(i));
    }

    for (auto* body : bodies)
    {
        world->destroyRigidBody(body);
    }

    std::cout << "RigidBodies after cleanup: "
        << world->getNbRigidBodies() << std::endl;

    //Recreate rigid bodies from ECS
    meshPhysicsQuery.each(
        [this, world, physicsCommon](
            flecs::entity e,
            TransfromComponent& transform,
            MeshComponent& mesh,
            PhysicsComponent& physics)
        {
            try
            {
                //Fetch collider data from GLTF
                ReactPhysics3DBoxColloider& boxCollider =
                    ReactPhysics3DLoader::GetReactPhysics3DBoxColloiderFromGLTFModel(
                        mesh.MeshName
                    );

                //Create RigidBody transform
                glm::quat entityRot =
                    glm::quat(glm::radians(transform.GetRotation()));

                rp3d::Transform bodyTransform(
                    rp3d::Vector3(
                        transform.GetPosition().x,
                        transform.GetPosition().y,
                        transform.GetPosition().z
                    ),
                    rp3d::Quaternion(
                        entityRot.x,
                        entityRot.y,
                        entityRot.z,
                        entityRot.w
                    )
                );

                physics.rigidBody = world->createRigidBody(bodyTransform);
                physics.rigidBody->setType(
                    static_cast<rp3d::BodyType>(physics.bodyType)
                );

                //Collider LOCAL transform
                glm::quat colliderRot =
                    glm::normalize(boxCollider.rotation);

                rp3d::Transform colliderTransform(
                    rp3d::Vector3(
                        boxCollider.positionOffset.x,
                        boxCollider.positionOffset.y,
                        boxCollider.positionOffset.z
                    ),
                    rp3d::Quaternion(
                        colliderRot.x,
                        colliderRot.y,
                        colliderRot.z,
                        colliderRot.w
                    )
                );

                //Create collider shape
                if (physics.colloiderType ==
                    PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER)
                {
                    rp3d::BoxShape* boxShape =
                        physicsCommon->createBoxShape({
                            boxCollider.halfExtents.x * transform.Scale.x,
                            boxCollider.halfExtents.y * transform.Scale.y,
                            boxCollider.halfExtents.z * transform.Scale.z
                            });

                    physics.rigidBody->addCollider(
                        boxShape,
                        colliderTransform
                    );
                }
                else if (physics.colloiderType ==
                    PHYSICSCOLLOIDERTYPE::SPHERECOLLOIDER)
                {
                    rp3d::SphereShape* sphereShape =
                        physicsCommon->createSphereShape(1.0f);

                    physics.rigidBody->addCollider(
                        sphereShape,
                        colliderTransform
                    );
                }

                //Debug flag
                physics.rigidBody->setIsDebugEnabled(
                    physics.IsDebugEnabled
                );
            }
            catch (const std::exception& ex)
            {
                std::cout
                    << "Physics error for entity '"
                    << e.name() << "': "
                    << ex.what() << std::endl;
            }
        }
    );
}


void ReactPhysics3DSystem::UpdateReactPhysics3DSystem()
{
    if (IsDebugEnabled != GlobalInformation::IsReactPhysics3DDebuggerEnabled) {
        reactPhysics3DCore->SetDebug(GlobalInformation::IsReactPhysics3DDebuggerEnabled);
        IsDebugEnabled = GlobalInformation::IsReactPhysics3DDebuggerEnabled;
    }

    if (GlobalInformation::IsReactPhysics3DStateCommited) {
        CommitReactPhysics3DState();
    }

    auto* world = reactPhysics3DCore->GetPhysicsWorld();
    auto physicsCommon = reactPhysics3DCore->GetPhysicsCommon();

    ecsWorld->observer<PhysicsComponent, TransfromComponent, MeshComponent>()
        .event(flecs::OnAdd)
        .each([this](flecs::entity e,
            PhysicsComponent& physics,
            TransfromComponent& transform,
            MeshComponent& mesh)
            {
                auto* world = reactPhysics3DCore->GetPhysicsWorld();
                auto physicsCommon = reactPhysics3DCore->GetPhysicsCommon();

                auto& boxCollider =
                    ReactPhysics3DLoader::GetReactPhysics3DBoxColloiderFromGLTFModel(
                        mesh.MeshName
                    );

                glm::quat rot = glm::quat(glm::radians(transform.GetRotation()));

                rp3d::Transform bodyTransform(
                    { transform.GetPosition().x,
                      transform.GetPosition().y,
                      transform.GetPosition().z },
                    { rot.x, rot.y, rot.z, rot.w }
                );

                physics.rigidBody = world->createRigidBody(bodyTransform);
                physics.rigidBody->setType(
                    static_cast<rp3d::BodyType>(physics.bodyType)
                );
                physics.rigidBody->setIsDebugEnabled(physics.IsDebugEnabled);
            });

    ecsWorld->observer<PhysicsComponent>()
        .event(flecs::OnRemove)
        .each([this](PhysicsComponent& physics)
            {
                if (physics.rigidBody) {
                    reactPhysics3DCore->GetPhysicsWorld()
                        ->destroyRigidBody(physics.rigidBody);
                    physics.rigidBody = nullptr;
                }
            });

    meshPhysicsQuery.each([this, &world, &physicsCommon](
        flecs::entity e,
        TransfromComponent& transfrom,
        MeshComponent& mesh,
        PhysicsComponent& physics
        )
        {
            
        });
}
