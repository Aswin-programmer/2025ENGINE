#include "Serialization.h"

std::string Serialization::serializationPath = "";

void Serialization::SaveScene( 
	const std::string& path, 
	SerializationMode mode,
    std::shared_ptr<flecs::world> ecsWorld
)
{
    SceneSnapshot snapshot;

    std::cout <<"Total Entities to be saved : "<< ecsWorld->count<IdentiferComponent>() << std::endl;

    ecsWorld->each<IdentiferComponent>([&](flecs::entity e, IdentiferComponent& transfromC) {
        SerializationContainer sc;

        if (e.name())
            sc.entityName = e.name();

        if (e.has<IdentiferComponent>())
        {
            sc.identiferComponent = e.get<IdentiferComponent>();
        }

        if (e.has<TransfromComponent>())
        {
            sc.transfromComponent = e.get<TransfromComponent>();
        }
        if (e.has<MeshComponent>())
        {
            sc.meshComponent = e.get<MeshComponent>();
        }
        if (e.has<AnimationComponent>())
        {
            sc.animationComponent = e.get<AnimationComponent>();
        }
        if (e.has<LightingComponent>())
        {
            sc.lightingComponent = e.get<LightingComponent>();
        }
        if (e.has<PhysicsComponent>())
        {
            sc.physicsComponent = e.get<PhysicsComponent>();
        }

        snapshot.entities.push_back(std::move(sc));
        });

    if (mode == SerializationMode::JSON)
    {
        std::ofstream os(path);
        cereal::JSONOutputArchive archive(os);
        archive(snapshot);
    }
    else
    {
        std::ofstream os(path, std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(snapshot);
    }
}

void Serialization::LoadScene(
    const std::string& path, 
    SerializationMode mode,
    std::shared_ptr<flecs::world> ecsWorld
)
{
	SceneSnapshot snapshot;

    if (mode == SerializationMode::JSON)
    {
        std::ifstream is(path);
        cereal::JSONInputArchive archive(is);
        archive(snapshot);
    }
    else
    {
        std::ifstream is(path, std::ios::binary);
        cereal::BinaryInputArchive archive(is);
        archive(snapshot);
    }

    for (const SerializationContainer& sc : snapshot.entities)
    {
        flecs::entity e = ecsWorld->entity(sc.entityName.c_str());
        if (sc.identiferComponent)
        {
            e.set<IdentiferComponent>(sc.identiferComponent.value());
        }
        if (sc.transfromComponent)
        {
            e.set<TransfromComponent>(sc.transfromComponent.value());
        }
        if (sc.meshComponent)
        {
            e.set<MeshComponent>(sc.meshComponent.value());
        }
        if (sc.animationComponent)
        {
            e.set<AnimationComponent>(sc.animationComponent.value());
        }
        if (sc.lightingComponent)
        {
            e.set<LightingComponent>(sc.lightingComponent.value());
        }
        if (sc.physicsComponent)
        {
            e.set<PhysicsComponent>(sc.physicsComponent.value());
        }
    }
}

void Serialization::SetSerializationPath(std::string serializationPath_)
{
    serializationPath = serializationPath_;
}
