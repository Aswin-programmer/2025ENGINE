#include "GlobalInformation.h"

bool GlobalInformation::isSunInitialized = false;
glm::vec3 GlobalInformation::sunLightDirection = glm::vec3(0.0f);
glm::vec3 GlobalInformation::sunLightPos = glm::vec3(0.0f);
glm::vec3 GlobalInformation::sunScreenCenter = glm::vec3(0.0f);
float GlobalInformation::sunLightDistanceFactor = 20.f;
bool GlobalInformation::renderShadowPass = false;
bool GlobalInformation::IsReactPhysics3DDebuggerEnabled = false;
bool GlobalInformation::IsReactPhysics3DStateCommited = false;
bool GlobalInformation::IsScenePlaying = false;
bool GlobalInformation::IsInGameUIRendered = false;

void GlobalInformation::InitSun()
{
    sunLightDirection = glm::normalize(glm::vec3(-0.2f, -1.f, -0.3f));
    sunScreenCenter = glm::vec3(0.f);

    sunLightPos = sunScreenCenter - sunLightDirection * sunLightDistanceFactor;
}

glm::mat4 GlobalInformation::GetSunViewMatrix()
{
    glm::vec3 sunTempLightDirection = glm::normalize(sunLightDirection);
    sunLightPos = sunScreenCenter - sunTempLightDirection * sunLightDistanceFactor;

    return glm::lookAt(sunLightPos, sunScreenCenter, glm::vec3(0.f, 1.f, 0.f));
}

