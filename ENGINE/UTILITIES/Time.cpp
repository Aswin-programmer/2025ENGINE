#include "Time.h"

float Time::timeStarted = glfwGetTime();

float Time::getTime()
{
    return (glfwGetTime() - timeStarted);
}
