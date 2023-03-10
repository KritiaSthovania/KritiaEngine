#include "ProjectFileExplorer.h"
#include "../EditorApplication.h"
#include "../../CoreModule/SceneManager.h"

using namespace std::filesystem;

void KritiaEngine::Editor::GUI::ProjectFileExplorer::OnGUI() {
	if (ImGui::TreeNodeEx("Assets", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (auto& v : directory_iterator("./Assets")) {
			if (is_directory(v.path())) {
				DisplayDirectory(v.path(), "./Assets/");
			} else {
				DisplayFile(v.path(), "./Assets/");
			}
		}
		ImGui::TreePop();
	}
}

void KritiaEngine::Editor::GUI::ProjectFileExplorer::DisplayDirectory(const path& path, const std::string& root) {
	if (ImGui::TreeNodeEx(path.filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (auto& v : directory_iterator(path)) {
			if (is_directory(v.path())) {
				DisplayDirectory(v.path(), root + path.filename().string());
			} else {
				DisplayFile(v.path(), root);
			}
		}
		ImGui::TreePop();
	}
}

void KritiaEngine::Editor::GUI::ProjectFileExplorer::DisplayFile(const path& fileName, const std::string& path) {
	if (ImGui::Selectable(fileName.filename().string().c_str(), true, ImGuiSelectableFlags_AllowDoubleClick) ) {
		if (ImGui::IsMouseDoubleClicked(0)) {
			// Double click different files
			if (fileName.filename().string().ends_with(KritiaEngine::Editor::sceneFilePostfix)) {
				SceneManagement::SceneManager::LoadScene(path + fileName.filename().string());
			}
		}
	}
}
