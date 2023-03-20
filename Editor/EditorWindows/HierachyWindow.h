#pragma once
#include "../EditorWindow.h"
#include "../../CoreModule/Manager/ResourceManager.h"

namespace KritiaEngine::Editor::GUI {
	class HierachyWindow : public KritiaEngine::Editor::EditorWindow {
	protected:
		void OnGUI();
		void Config();
	private:
		static void CreateMeshPrimitive(KritiaEngine::Manager::ResourceManager::MeshPrimitive type, GameObject* obj);
		static std::list<GameObject*> objectToDelete;
	};
}


