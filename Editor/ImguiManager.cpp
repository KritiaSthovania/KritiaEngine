#include "ImguiManager.h"
#include "EditorWindow.h"
#include "EditorWindows/HierachyWindow.h"
#include "EditorWindows/InspectorWindow.h"
#include "EditorWindows/ProjectFileExplorer.h"
#include "MainMenuBarFunction.h"
#include "../CoreModule/Settings.h"
#include <nfd/nfd.h>

using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

const char* ImguiManager::GlslVersion = "#version 130";
bool ImguiManager::inEditor = true;
float ImguiManager::UIScaleFactor = 2;
KritiaEngine::IInspectable* ImguiManager::currentSelectedInspectable;
std::list<std::shared_ptr<EditorWindow>> ImguiManager::editorWindows = std::list<std::shared_ptr<EditorWindow>>();
//////////////////// window opened  ////////////////////
bool ImguiManager::settingWindowOpened = false;

void KritiaEngine::Editor::GUI::ImguiManager::Initialize(GLFWwindow* window, bool inEditor) {
	ImguiManager::inEditor = inEditor;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	(void)IO;
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	IO.FontGlobalScale = UIScaleFactor;
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GlslVersion);
	CreateEditorWindows();
}

void KritiaEngine::Editor::GUI::ImguiManager::RenderGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
	if (inEditor) {
		RenderMainMenuBar();
		RenderWindowOpenedFromMainMenuBar();
		for (std::shared_ptr<EditorWindow> window : editorWindows) {
			//window->Config();
			ImGui::SetNextWindowBgAlpha(1);
			ImGui::Begin(window->title, 0, window->GetFlags());
			window->OnGUI();
			ImGui::End();
		}
	}

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void KritiaEngine::Editor::GUI::ImguiManager::AddEditorWindow(std::shared_ptr<EditorWindow> window) {
	editorWindows.push_back(window);
}

void KritiaEngine::Editor::GUI::ImguiManager::RemoveEditorWindow(std::shared_ptr<EditorWindow> window) {
	editorWindows.remove(window);
}

void KritiaEngine::Editor::GUI::ImguiManager::RenderWindowOpenedFromMainMenuBar() {
	if (settingWindowOpened) {
		MainMenuBarFunction::OpenSettingWindow(&settingWindowOpened);
	}
}

void KritiaEngine::Editor::GUI::ImguiManager::CreateEditorWindows() {
	EditorWindow::GetWindow<HierachyWindow>("Hiearachy");
	EditorWindow::GetWindow<InspectorWindow>("Inspector");
	EditorWindow::GetWindow<ProjectFileExplorer>("Project");
}

void KritiaEngine::Editor::GUI::ImguiManager::RenderMainMenuBar() {
	ImGui::BeginMainMenuBar();
	if (ImGui::MenuItem("Open Scene")) {
		MainMenuBarFunction::OpenScene();
	}
	if (ImGui::MenuItem("Save Scene")) {
		MainMenuBarFunction::SaveScene();
	}
	if (ImGui::MenuItem("Settings")) {
		settingWindowOpened = true;
	}
	if (ImGui::MenuItem("Import Asset")) {
		MainMenuBarFunction::OpenImportAssetWindow();
	}
	ImGui::EndMainMenuBar();
}
