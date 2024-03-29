#include "ProjectFileExplorer.h"
#include "../EditorApplication.h"
#include "../../CoreModule/Manager/ResourceManager.h"
#include "../../CoreModule/SceneManager.h"
#include "../../CoreModule/Material.h"

using namespace std::filesystem;
using namespace KritiaEngine::Manager;

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
	// Right Click
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
		ImGui::OpenPopup("RightClickDirectory");
	}
	if (ImGui::BeginPopup("RightClickDirectory")) {
		ShowRightClickMenu();
		ImGui::EndPopup();
	}
}

void KritiaEngine::Editor::GUI::ProjectFileExplorer::DisplayDirectory(const path& path, const std::string& root) {
	if (ImGui::TreeNodeEx(path.filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (auto& v : directory_iterator(path)) {
			if (is_directory(v.path())) {
				DisplayDirectory(v.path(), root + path.filename().string());
			} else {
				DisplayFile(v.path().filename(), path.string() + "/");
			}
		}
		ImGui::TreePop();
	}
}

void KritiaEngine::Editor::GUI::ProjectFileExplorer::DisplayFile(const path& fileName, const std::string& path) {
	if (ImGui::Selectable(fileName.filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick) ) {
		std::string completePath = path + fileName.filename().string();
		if (fileName.filename().string().ends_with(KritiaEngine::Editor::materialFilePostfix)) {
			ImguiManager::currentSelectedInspectable = (IInspectable*)ResourceManager::GetMaterial(completePath).get();
		}		
		if (fileName.filename().string().ends_with(KritiaEngine::Editor::textureFilePostfix)) {
			ImguiManager::currentSelectedInspectable = (IInspectable*)ResourceManager::GetTexture(completePath).get();
		}
		if (fileName.filename().string().ends_with(KritiaEngine::Editor::meshFilePostfix)) {
			ImguiManager::currentSelectedInspectable = (IInspectable*)ResourceManager::GetMesh(completePath).get();
		}
		if (fileName.filename().string().ends_with(KritiaEngine::Editor::prefabFilePostfix)) {
			ImguiManager::currentSelectedInspectable = (IInspectable*)ResourceManager::GetPrefab(completePath);
		}
		// Double left click on different files
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (fileName.filename().string().ends_with(KritiaEngine::Editor::sceneFilePostfix)) {
				SceneManagement::SceneManager::LoadScene(completePath);
			}
			if (fileName.filename().string().ends_with(KritiaEngine::Editor::prefabFilePostfix)) {
				GameObject::DeserializeFromPath(completePath);
			}
		}
	}
}

void KritiaEngine::Editor::GUI::ProjectFileExplorer::ShowRightClickMenu() {
	if (ImGui::BeginMenu("Create")) {
		if (ImGui::Selectable("New Material")) {
			Material mat = Material("");
			mat.SerializeToFile();
		}
		ImGui::EndMenu();
	}

}
