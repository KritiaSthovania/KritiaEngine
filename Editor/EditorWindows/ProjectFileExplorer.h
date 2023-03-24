#pragma once
#include "../EditorWindow.h"
#include "../EditorApplication.h"
#include <filesystem>

using namespace std::filesystem;
using namespace KritiaEngine::Editor;

namespace KritiaEngine::Editor::GUI {

	class ProjectFileExplorer : public EditorWindow {
	protected:
		void OnGUI();
	private:
		std::unique_ptr<path> rootPath = std::make_unique<path>(path(EditorApplication::currentProjectFolderPath));
		void DisplayDirectory(const path& relativePath, const std::string& root);
		void DisplayFile(const path& fileName, const std::string& rootPath);
		void ShowRightClickMenu();
	};

}

