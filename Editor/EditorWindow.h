#pragma once
#include <imgui/imgui.h>
#include "../CoreModule/Object.h"
#include "ImguiManager.h"

namespace KritiaEngine::Editor {
	class EditorWindow : public Object {
		friend class KritiaEngine::Editor::GUI::ImguiManager;
	public:
		template <typename T> static std::shared_ptr<T> CreateWindow(const char* title) {
			std::shared_ptr<T> window = std::make_shared<T>(T());
			window->title = title;
			KritiaEngine::Editor::GUI::ImguiManager::AddEditorWindow(window);
			return window;
		}
		const char* title;
	protected:
		virtual void OnGUI();
	private:
		void OnObjectDestroy();
	};
}


