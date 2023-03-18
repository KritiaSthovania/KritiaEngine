#pragma once
#include <imgui/imgui.h>
#include "../CoreModule/Object.h"
#include "ImguiManager.h"
#include "../CoreModule/MathStructs.h"

namespace KritiaEngine::Editor {
	class EditorWindow : public Object {
		friend class KritiaEngine::Editor::GUI::ImguiManager;
	public:
		/// <summary>
		/// Create a window with the given title.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="title"></param>
		/// <returns></returns>
		template <typename T> static std::shared_ptr<T> GetWindow(const char* title) {
			std::shared_ptr<T> window = std::make_shared<T>(T());
			window->title = title;
			KritiaEngine::Editor::GUI::ImguiManager::AddEditorWindow(window);
			return window;
		}
		const char* title;
	protected:
		virtual void OnGUI();
		/// <summary>
		/// Define defualt configuration here, e.g. size, pos
		/// </summary>
		virtual void Config();
		virtual int GetFlags();
	private:
		void OnObjectDestroy();
	};
}


