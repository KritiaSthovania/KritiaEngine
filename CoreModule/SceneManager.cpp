#include "SceneManager.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/Manager/RendererManager.h"
#include <json/json.hpp>

using json = nlohmann::ordered_json;
using namespace KritiaEngine::SceneManagement;
using namespace KritiaEngine::Manager;

std::shared_ptr<Scene> SceneManager::activeScene = nullptr;
bool SceneManager::inEditor = false;

/// <summary>
/// Initialize the scene manager
/// </summary>
/// <param name="inEditor">If we are in the Editor or Game</param>
void KritiaEngine::SceneManagement::SceneManager::Initialize(bool inEditor)
{
	// Ŀǰֱ�Ӽ��ز����õĳ���
	activeScene = CreateScene("Demo Scene");
	LoadScene(activeScene);
	//LoadScene(KritiaEngine::Editor::EditorApplication::AssetFolderRootPath + "Demo Scene" + KritiaEngine::Editor::sceneFilePostfix);
	SceneManager::inEditor = inEditor;
	if (inEditor) {
		Camera::editorCamera = std::shared_ptr<Camera>(new Camera());
		Camera::current = Camera::editorCamera;
	}
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::CreateScene(const std::string &name)
{
	// Ŀǰ�����Ǵ����ļ�
	return std::shared_ptr<Scene>(new Scene(name));
}

void KritiaEngine::SceneManagement::SceneManager::LoadScene(const std::shared_ptr<Scene>& scene)
{
	// Ŀǰ�����Ƕ�ȡ�ļ�
	scene->Initialize();
	scene->path = "";
}

void KritiaEngine::SceneManagement::SceneManager::LoadScene(const std::string& path) {
	BehaviourManager::Clear();
	RendererManager::Clear();
	activeScene = std::shared_ptr<Scene>(new Scene("", path));
	activeScene->Initialize();
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::GetActiveScene() {
	return activeScene;

}
