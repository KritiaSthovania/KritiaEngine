#pragma once
#include <pch.h>
#include <string>
namespace KritiaEngine::Editor {
	constexpr const char* sceneFilePostfix = ".scene.json";
	constexpr const char* materialFilePostfix = ".mat.json";
	constexpr const char* meshFilePostfix = ".mesh.json";
	constexpr const char* prefabFilePostfix = ".prefab.json";
	constexpr const char* textureFilePostfix = ".tex.json";
	class EditorApplication {
	public:
		static const std::string assetFolderRootPath;
		static std::string currentProjectFolderPath;
		static bool isPlaying;
	};
}



