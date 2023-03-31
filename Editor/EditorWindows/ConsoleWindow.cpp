#include "ConsoleWindow.h"
#include "../../CoreModule/Utilities.h"
using namespace KritiaEngine;

void KritiaEngine::Editor::GUI::ConsoleWindow::OnGUI() {
	if (Debug::logStream.str() != "") {
		logStrings.push_back(Debug::logStream.str());
		Debug::logStream.str("");
	}
	ImGui::BeginMenuBar();
	ImGui::Selectable(std::format("Log: {}", std::to_string(logStrings.size())).c_str());
	if (ImGui::Button("Clear")) {
		logStrings.clear();
	}
	ImGui::Checkbox("Scroll", &scrolling);
	ImGui::EndMenuBar();
	ImGui::PushID("##Console");
	for (int i = 0; i<logStrings.size(); i++){
		if (ImGui::Selectable(std::format("Info: {}", logStrings[i]).c_str())) {
			scrolling = false;
		}
	}
	if (scrolling) {
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::PopID();
}

int KritiaEngine::Editor::GUI::ConsoleWindow::GetFlags() {
	return ImGuiWindowFlags_MenuBar;
}

