#pragma once
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <pch.h>

namespace KritiaEngine {
	class GameObject;
}

namespace KritiaEngine::Editor {
	class EditorWindow;
}

namespace KritiaEngine::Editor::GUI {
	class ImguiManager {
	public:
		static void Initialize(GLFWwindow* window, bool inEditor);
		static void RenderGUI();
		static void AddEditorWindow(std::shared_ptr<EditorWindow> window);
		static void RemoveEditorWindow(std::shared_ptr<EditorWindow> window);
		static std::shared_ptr<GameObject> currentSelectedGameObject;
	private:
		static void CreateEditorWindows();
		static bool inEditor;
		static const char* GlslVersion;
		static float UIScaleFactor;
		static std::list<std::shared_ptr<EditorWindow>> editorWindows;
	};
	
}


