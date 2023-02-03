#include "HierachyWindow.h"
#include "../../CoreModule/SceneManager.h"

using namespace KritiaEngine::SceneManagement;

void KritiaEngine::Editor::GUI::HierachyWindow::OnGUI() {
	for (std::shared_ptr<GameObject> gameObject : SceneManager::GetActiveScene()->GetRootGameObjects()) {
		ImGui::Text(gameObject->name);
	}
}
