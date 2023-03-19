#include "HierachyWindow.h"
#include "../../CoreModule/SceneManager.h"
#include "../../CoreModule/Settings.h"
#include "../ImguiManager.h"

using namespace KritiaEngine::SceneManagement;

void KritiaEngine::Editor::GUI::HierachyWindow::OnGUI() {
	if (ImGui::TreeNodeEx(SceneManager::GetActiveScene()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (GameObject* gameObject : SceneManager::GetActiveScene()->GetRootGameObjects()) {
			if (ImGui::TreeNodeEx(gameObject->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth)) {
				if (ImGui::IsItemToggledOpen() || ImGui::IsItemClicked()) {
					ImguiManager::currentSelectedGameObject = gameObject;
				}
				ImGui::TreePop();
			} else {
				if (ImGui::IsItemClicked()) {
					ImguiManager::currentSelectedGameObject = gameObject;
				}
			}
		}
		ImGui::TreePop();
	}
}

void KritiaEngine::Editor::GUI::HierachyWindow::Config() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(Settings::ScreenWidth / 6, Settings::ScreenHeight));
}
