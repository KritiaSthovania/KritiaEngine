#pragma once
#include "../EditorWindow.h"


namespace KritiaEngine::Editor::GUI {
	class ConsoleWindow : public EditorWindow {
		// Í¨¹ý EditorWindow ¼Ì³Ð
	protected:
		virtual void OnGUI() override;
		virtual int GetFlags() override;
	private:
		std::vector<std::string> logStrings;
		bool scrolling = true;
		int count;
	};

}


