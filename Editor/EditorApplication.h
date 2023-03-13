#pragma once
#include <pch.h>
namespace KritiaEngine::Editor {
	constexpr const char* sceneFilePostfix = ".scene.json";
	constexpr const char* materialFilePostfix = ".mat.json";
	constexpr const char* meshFilePostfix = ".mesh.json";
	class EditorApplication {
	public:
		static const std::string assetFolderRootPath;
		static std::string currentProjectFolderPath;
	};
}



