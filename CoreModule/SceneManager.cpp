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
	// 目前直接生成测试用的场景
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
	// 目前不考虑创建文件
	return std::shared_ptr<Scene>(new Scene(name));
}

void KritiaEngine::SceneManagement::SceneManager::LoadScene(std::shared_ptr<Scene> scene)
{
	// 目前不考虑读取文件
	scene->Initialize();
}

void KritiaEngine::SceneManagement::SceneManager::Render() {
	if (inEditor) {
		activeScene->Render(Camera::editorCamera);
	} else {
		activeScene->Render(Camera::current);
	}
}

void KritiaEngine::SceneManagement::SceneManager::Update()
{
	if (inEditor) {
		Camera::editorCamera->EditorCameraUpdate();
	}
	activeScene->Update();
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::GetActiveScene() {
	return activeScene;
}
