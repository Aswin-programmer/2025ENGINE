#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ENGINE_API GlobalInformation
{
public:
    GlobalInformation(const GlobalInformation&) = delete;
    GlobalInformation& operator=(const GlobalInformation) = delete;

    static void InitSun();
    static glm::mat4 GetSunViewMatrix();

    // Variables for Sun Position
    static glm::vec3 sunLightDirection;
    static glm::vec3 sunLightPos;
    static glm::vec3 sunScreenCenter;
    static float sunLightDistanceFactor;
    static bool isSunInitialized;

    static bool renderShadowPass;

    static bool IsReactPhysics3DDebuggerEnabled;
    static bool IsReactPhysics3DStateCommited;
    static bool IsScenePlaying;

};
