#pragma once
#include "Interface/ISerializable.h"
#include "../Rendering/RenderingProvider.h"

namespace KritiaEngine{
	class Settings : public IJsonSerializable, public IJsonDeserializable {
	public:
		static int ScreenWidth;
		static int ScreenHeight;
		static float NearPlaneDistance;
		static float FarPlaneDistance;
		static KritiaEngine::Rendering::RenderingProvider::RenderingBackend renderingBackend;
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
