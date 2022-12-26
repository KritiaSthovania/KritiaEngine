#include "SceneManager.h"

using namespace KritiaEngine::SceneManagement;

std::shared_ptr<Scene> SceneManager::activeScene = nullptr;
bool SceneManager::inEditor = false;

/// <summary>
/// Initialize the scene manager
/// </summary>
/// <param name="inEditor">If we are in the Editor or Game</param>
void KritiaEngine::SceneManagement::SceneManager::Initialize(bool inEditor)
{
	// Ŀǰֱ�����ɲ����õĳ���
	activeScene = CreateScene("Demo Scene");
	LoadScene(activeScene);
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
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::GetActiveScene() {
	return activeScene;
}
