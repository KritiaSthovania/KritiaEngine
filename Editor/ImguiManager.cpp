#include "ImguiManager.h"
#include "EditorWindow.h"
#include "EditorWindows/HierachyWindow.h"
#include "EditorWindows/InspectorWindow.h"

using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

const char* ImguiManager::GlslVersion = "#version 130";
bool ImguiManager::inEditor = true;
float ImguiManager::UIScaleFactor = 2;
std::shared_ptr<KritiaEngine::GameObject> ImguiManager::currentSelectedGameObject = nullptr;
std::list<std::shared_ptr<EditorWindow>> ImguiManager::editorWindows = std::list<std::shared_ptr<EditorWindow>>();

void KritiaEngine::Editor::GUI::ImguiManager::Initialize(GLFWwindow* window, bool inEditor) {
	ImguiManager::inEditor = inEditor;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	(void)IO;
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	IO.FontGlobalScale = UIScaleFactor;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GlslVersion);
	CreateEditorWindows();
}

void KritiaEngine::Editor::GUI::ImguiManager::RenderGUI() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	if (inEditor) {
		for (std::shared_ptr<EditorWindow> window : editorWindows) {
			window->Config();
			ImGui::SetNextWindowBgAlpha(1);
			ImGui::Begin(window->title, 0, window->flags);
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
	EditorWindow::CreateWindow<InspectorWindow>("Inspector");
}