#include "InspectorWindow.h"
#include "../../CoreModule/Settings.h"
#include "../ImguiManager.h"
#include "../../Component/Component.h"

void KritiaEngine::Editor::GUI::InspectorWindow::OnGUI() {
	if (ImguiManager::currentSelectedGameObject != nullptr) {
		for (std::shared_ptr<Component> comp : ImguiManager::currentSelectedGameObject->components) {
			comp->OnInspector();
		}
	}
}

void KritiaEngine::Editor::GUI::InspectorWindow::Config() {
	ImGui::SetNextWindowPos(ImVec2(Settings::ScreenWidth / 6 * 5, 0));
	ImGui::SetNextWindowSize(ImVec2(Settings::ScreenWidth / 6, Settings::ScreenHeight));
}
