#include "Time.h"

using namespace KritiaEngine;

float Time::deltaTime = 0.f;
float Time::lastFrameTime = 0.f;

float KritiaEngine::Time::GetTime()
{
	return glfwGetTime();
}

void KritiaEngine::Time::UpdateDeltaTime(float currentTime)
{
	deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;
}
