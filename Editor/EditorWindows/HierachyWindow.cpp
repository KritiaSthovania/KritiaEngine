#include "HierachyWindow.h"
#include "../../CoreModule/SceneManager.h"
#include "../../CoreModule/Settings.h"
#include "../ImguiManager.h"
#include "../../Component/MeshFilter.h"
#include "../../Component/Renderer/MeshRenderer.h"

using namespace KritiaEngine::Manager;
using namespace KritiaEngine::SceneManagement;

void KritiaEngine::Editor::GUI::HierachyWindow::OnGUI() {
	bool objClicked = false;
	int objecIndex = 0;
	if (ImGui::TreeNodeEx(SceneManager::GetActiveScene()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (GameObject* gameObject : SceneManager::GetActiveScene()->GetRootGameObjects()) {
			if (ImGui::TreeNodeEx(gameObject->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth)) {
				if (ImGui::IsItemToggledOpen() || ImGui::IsItemClicked()) {
					ImguiManager::currentSelectedInspectable = (IInspectable*)gameObject;
				}
				ImGui::TreePop();
			} else {
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					ImguiManager::currentSelectedInspectable = (IInspectable*)gameObject;
				} else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup("RightClickMenuHierachyObject" + objecIndex);
					objClicked = true;

				}
			}
			if (ImGui::BeginPopup("RightClickMenuHierachyObject" + objecIndex)) {
				if (ImGui::Selectable("Delete")) {
					objectToDelete.push_back(gameObject);
					ImGui::CloseCurrentPopup();
					ImguiManager::currentSelectedInspectable = nullptr;
				}
				ImGui::EndPopup();
			}
			objecIndex++;
		}
		for (GameObject* go : objectToDelete) {
			Destroy(go);
		}
		objectToDelete.clear();
		ImGui::TreePop();
	}
	if (!objClicked) {
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("RightClickMenuHierachy");
		}
		if (ImGui::BeginPopup("RightClickMenuHierachy")) {
			ShowRightClickMenu();
			ImGui::EndPopup();
		}
	}

}

void KritiaEngine::Editor::GUI::HierachyWindow::Config() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(Settings::ScreenWidth / 6, Settings::ScreenHeight));
}

void KritiaEngine::Editor::GUI::HierachyWindow::CreateMeshPrimitive(KritiaEngine::Manager::ResourceManager::MeshPrimitive type, GameObject* obj) {
	obj->AddComponent<MeshFilter>();
	obj->GetComponent<MeshFilter>()->mesh = ResourceManager::GetMeshPrimitive(ResourceManager::MeshPrimitive::Cube);
	obj->AddComponent<MeshRenderer>();
}

void KritiaEngine::Editor::GUI::HierachyWindow::ShowRightClickMenu() {

	if (ImGui::Selectable("Empty GameObject")) {
		GameObject* gameObject = new GameObject("New GameObject");
		ImGui::CloseCurrentPopup();
	}
	if (ImGui::BeginMenu("3D Models")) {
		if (ImGui::Button("Cube")) {
			GameObject* gameObject = new GameObject("Cube");
			CreateMeshPrimitive(ResourceManager::MeshPrimitive::Cube, gameObject);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndMenu();
	}
}