#include "ImguiAlias.h"
#include <format>

int KritiaEngine::Editor::GUI::ImguiAlias::treeFlagDefaultOpenAndSpanFullWidth = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;

void KritiaEngine::Editor::GUI::ImguiAlias::FloatField(const char* label, float* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::InputFloat(std::format("##{}\n", label).c_str(), value);
}

void KritiaEngine::Editor::GUI::ImguiAlias::FloatField3(const char* label, float* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::InputFloat3(std::format("##{}\n", label).c_str(), value);
}

void KritiaEngine::Editor::GUI::ImguiAlias::FloatField4(const char* label, float* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::InputFloat4(std::format("##{}\n", label).c_str(), value);
}

void KritiaEngine::Editor::GUI::ImguiAlias::BoolField(const char* label, bool* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::Checkbox(std::format("##{}\n", label).c_str(), value);
}

void KritiaEngine::Editor::GUI::ImguiAlias::ColorField3(const char* label, float* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::ColorEdit3(std::format("##{}\n", label).c_str(), value);
}
