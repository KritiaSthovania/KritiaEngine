#include "ImguiManager.h"
#include "EditorWindow.h"
#include "EditorWindows/HierachyWindow.h"
#include "EditorWindows/InspectorWindow.h"
#include "EditorWindows/ProjectFileExplorer.h"
#include "EditorWindows/ConsoleWindow.h"
#include "MainMenuBarFunction.h"
#include "../CoreModule/Settings.h"
#include "../CoreModule/Input.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Rendering/RenderingProvider.h"
#include <nfd/nfd.h>

using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

const char* ImguiManager::GlslVersion = "#version 130";
bool ImguiManager::inEditor = true;
float ImguiManager::UIScaleFactor = 2;
KritiaEngine::IInspectable* ImguiManager::currentSelectedInspectable;
std::list<std::shared_ptr<EditorWindow>> ImguiManager::editorWindows = std::list<std::shared_ptr<EditorWindow>>();
ImGuizmo::OPERATION ImguiManager::operation = ImGuizmo::OPERATION::TRANSLATE;
ImGuizmo::MODE ImguiManager::mode = ImGuizmo::MODE::WORLD;

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
	ImGuizmo::BeginFrame();
	if (inEditor) {
		RenderMainMenuBar();
		RenderWindowOpenedFromMainMenuBar();
		for (std::shared_ptr<EditorWindow> window : editorWindows) {
			//window->Config();
			ImGui::SetNextWindowBgAlpha(1);
			ImGui::Begin(window->title, 0, window->GetFlags() | ImGuiWindowFlags_AlwaysAutoResize);
			window->OnGUI();
			ImGui::End();
		}

		// Not Working
		GameObject* go = dynamic_cast<GameObject*>(currentSelectedInspectable);
		if (go != nullptr) {
			ShowImGuizmo(go);
		} else {
			ImGuizmo::Enable(false);
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
	EditorWindow::GetWindow<ConsoleWindow>("Console");
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

// Not Working
void KritiaEngine::Editor::GUI::ImguiManager::ShowImGuizmo(GameObject* go) {
	ImGuizmo::Enable(true);
	if (Input::GetKeyDown(GLFW_KEY_W)) {
		operation = ImGuizmo::TRANSLATE;
	}
	if (Input::GetKeyDown(GLFW_KEY_E)) {
		operation = ImGuizmo::ROTATE;
	}
	if (Input::GetKeyDown(GLFW_KEY_R)) {
		operation = ImGuizmo::SCALE;
	}

	Matrix4x4 model = Matrix4x4::Identity();
	model = Mathf::Translate(model, go->Transform()->position);
	model *= Quaternion::ToRotationMatrix(go->Transform()->rotation);
	model = Mathf::Scale(model, go->Transform()->scale);
	if(ImGuizmo::IsUsing()) {
		std::cout << "IsUsing";
	}
	ImGuizmo::AllowAxisFlip(false);
	ImGuizmo::SetRect(0, 0, Settings::ScreenWidth, Settings::ScreenHeight);
	//ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
	ImGuizmo::RecomposeMatrixFromComponents(&(go->Transform()->position.x), &(go->Transform()->rotationEuler.x), &(go->Transform()->scale.x), model.GetPtr());
	ImGuizmo::Manipulate(Camera::current->GetViewMatrix().GetPtr(), Rendering::RenderingProvider::projection.GetPtr(), operation, mode, model.GetPtr());
	ImGuizmo::DecomposeMatrixToComponents(model.GetPtr(), &(go->Transform()->position.x), &(go->Transform()->rotationEuler.x), &(go->Transform()->scale.x));
}