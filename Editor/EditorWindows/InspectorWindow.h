#pragma once
#include "../EditorWindow.h"

namespace KritiaEngine::Editor::GUI {
	class InspectorWindow : public KritiaEngine::Editor::EditorWindow {
	public:

	protected:
		void OnGUI();
		void Config();
	};
}


