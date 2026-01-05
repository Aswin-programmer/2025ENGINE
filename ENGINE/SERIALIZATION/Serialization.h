#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <optional>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>     // std::string
#include <cereal/types/vector.hpp>     // std::vector
#include <cereal/types/optional.hpp>

#include "../ECS/ECSWorld.h"
#include "../ECS/COMPONENTS/IdentiferComponent.h"
#include "../ECS/COMPONENTS/TransfromComponent.h"
#include "../ECS/COMPONENTS/PhysicsComponent.h"
#include "../ECS/COMPONENTS/AnimationComponent.h"
#include "../ECS/COMPONENTS/MeshComponent.h"
#include "../ECS/COMPONENTS/LightingComponent.h"

enum class ENGINE_API SerializationMode {
	BINARY,
	JSON
};

struct ENGINE_API SerializationContainer
{
    std::string entityName;

    std::optional<IdentiferComponent> identiferComponent;
    std::optional<TransfromComponent> transfromComponent;
    std::optional<MeshComponent> meshComponent;
    std::optional<AnimationComponent> animationComponent;
    std::optional<LightingComponent> lightingComponent;
    std::optional<PhysicsComponent> physicsComponent;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(
            CEREAL_NVP(entityName),
            CEREAL_NVP(identiferComponent),
            CEREAL_NVP(transfromComponent),
            CEREAL_NVP(meshComponent),
            CEREAL_NVP(animationComponent),
            CEREAL_NVP(lightingComponent),
            CEREAL_NVP(physicsComponent)
        );
    }
};

struct ENGINE_API SceneSnapshot
{
	std::vector<SerializationContainer> entities;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(entities));
	}
};

class ENGINE_API Serialization
{
public:
	static void SaveScene(
		const std::string& path,
		SerializationMode mode,
        std::shared_ptr<flecs::world> ecsWorld
	);

	static void LoadScene(
		const std::string& path,
		SerializationMode mode,
        std::shared_ptr<flecs::world> ecsWorld
	);

    static void SetSerializationPath(std::string serializationPath_);
    
    static std::string serializationPath;
};
