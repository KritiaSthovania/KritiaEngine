#include "ImguiManager.h"
#include "EditorWindow.h"
#include "EditorWindows/HierachyWindow.h"

using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

const char* ImguiManager::GlslVersion = "#version 130";
bool ImguiManager::inEditor = true;
std::list<std::shared_ptr<EditorWindow>> ImguiManager::editorWindows = std::list<std::shared_ptr<EditorWindow>>();

void KritiaEngine::Editor::GUI::ImguiManager::Initialize(GLFWwindow* window, bool inEditor) {
	ImguiManager::inEditor = inEditor;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	(void)IO;
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GlslVersion);
	CreateEditorWindows();
}

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void KritiaEngine::Editor::GUI::ImguiManager::RenderGUI() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	if (inEditor) {
		for (std::shared_ptr<EditorWindow> window : editorWindows) {
			ImGui::Begin(window->title);
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

void KritiaEngine::Editor::GUI::ImguiManager::CreateEditorWindows() {
	EditorWindow::CreateWindow<HierachyWindow>("Hiearachy");
}
