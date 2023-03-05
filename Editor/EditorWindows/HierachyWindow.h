#pragma once
#include "../EditorWindow.h"

namespace KritiaEngine::Editor::GUI {
	class HierachyWindow : public KritiaEngine::Editor::EditorWindow {
	public:
		HierachyWindow();
	protected:
		void OnGUI();
		void Config();
	};
}

