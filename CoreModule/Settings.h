#pragma once
#include "Interface/SerializableAndDeserializable.h"

namespace KritiaEngine{
	class Settings {
	public:
		static int ScreenWidth;
		static int ScreenHeight;
		static float NearPlaneDistance;
		static float FarPlaneDistance;
		static bool UseOpenGL;
		static bool UseGLM;
		static int ShadowWidth, ShadowHeight;
		static std::string LastOpenedScenePath;
		// 通过 JsonSerializable 继承
		static void Serialize();

		// 通过 PathDeserializable 继承
		static void Deserialize();

		static void ShowOnEditorWindow();
	};
}
