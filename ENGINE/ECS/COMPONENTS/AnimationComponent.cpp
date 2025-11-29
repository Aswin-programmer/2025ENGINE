#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(bool isAnimationEnabled_)
	:
	isAnimationEnabled{isAnimationEnabled_},
	currentTime{ 0 }
{

}

double AnimationComponent::GetCurrentOnTime()
{
	if (!isAnimationEnabled) return 0;

	currentTime = glfwGetTime();
	currentTime += Window::getdt();
	return currentTime;
}
