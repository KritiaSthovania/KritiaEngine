#include "ImguiAlias.h"
#include "EditorApplication.h"
#include <nfd/nfd.h>
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

void KritiaEngine::Editor::GUI::ImguiAlias::IntField(const char* label, int* value) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::InputInt(std::format("##{}\n", label).c_str(), value);
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

std::string KritiaEngine::Editor::GUI::ImguiAlias::OpenFindResourceWindow(const char* resourceType, const char* filePostfix) {
	NFD_Init();
	// Remove the dot
	if (filePostfix[0] == '.') {
		filePostfix++;
	}
	nfdchar_t* outPath;
	nfdfilteritem_t filterItem[1] = { { resourceType, filePostfix}};
	nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
	if (result == NFD_OKAY) {
		std::string out = outPath;
		NFD_FreePath(outPath);
		NFD_Quit();
		return out;
	} else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
	} else {
		printf("Error: %s\n", NFD_GetError());
	}
	NFD_Quit();
	return "";
}

std::string KritiaEngine::Editor::GUI::ImguiAlias::OpenSaveResourceWindow(const char* resourceType, const char* filePostfix, const char* defaultName) {
	NFD_Init();
	// Remove the dot
	if (filePostfix[0] == '.') {
		filePostfix++;
	}
	nfdchar_t* outPath;
	nfdfilteritem_t filterItem[1] = { { resourceType, filePostfix} };
	nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, (EditorApplication::currentProjectFolderPath + EditorApplication::assetFolderRootPath).c_str(), defaultName);
	if (result == NFD_OKAY) {
		std::string out = outPath;
		NFD_FreePath(outPath);
		NFD_Quit();
		return out;
	} else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
	} else {
		printf("Error: %s\n", NFD_GetError());
	}
	NFD_Quit();
	return "";
}
