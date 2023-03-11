#pragma once
#include "../EditorWindow.h"

namespace KritiaEngine::Editor::GUI {
	class InspectorWindow : public KritiaEngine::Editor::EditorWindow {
	protected:
		void OnGUI();
		void Config();
	};
}


