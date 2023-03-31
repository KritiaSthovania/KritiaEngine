#include "EditorWindow.h"
using namespace KritiaEngine::Editor::GUI;
void KritiaEngine::Editor::EditorWindow::Config() {}

int KritiaEngine::Editor::EditorWindow::GetFlags() {
	return ImGuiWindowFlags_None;
}

void KritiaEngine::Editor::EditorWindow::OnObjectDestroy() {
	
}
