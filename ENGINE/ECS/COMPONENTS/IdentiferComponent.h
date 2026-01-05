#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <cereal/cereal.hpp>

struct ENGINE_API IdentiferComponent
{
	IdentiferComponent() = default;
	IdentiferComponent(std::string entityName);

	~IdentiferComponent() = default;

	std::string entityName;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(
            CEREAL_NVP(entityName)
        );
    }
};

