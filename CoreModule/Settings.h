#pragma once
#include "Interface/ISerializable.h"

namespace KritiaEngine{
	class Settings : public IJsonSerializable, public IJsonDeserializable {
	public:
		static int ScreenWidth;
		static int ScreenHeight;
		static float NearPlaneDistance;
		static float FarPlaneDistance;
		static bool UseOpenGL;
		static bool UseGLM;
		static int ShadowWidth, ShadowHeight;
		static std::string LastOpenedScenePath;
		// ͨ�� JsonSerializable �̳�
		static void Serialize();

		// ͨ�� PathDeserializable �̳�
		static void Deserialize();

		static void ShowOnEditorWindow();
	};
}
