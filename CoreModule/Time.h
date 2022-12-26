#pragma once
#include <pch.h>
namespace KritiaEngine {
	class Time
	{
	public:
		/// <summary>
		/// Get the time in seconds since the application is started.
		/// </summary>
		/// <returns></returns>
		static float GetTime();
		static void UpdateDeltaTime(float currentTime);
		static float deltaTime;
	private:
		static float lastFrameTime;
	};
}


