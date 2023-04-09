#include "SceneManager.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/Manager/RendererManager.h"
#include "../CoreModule/Manager/GameObjectManager.h"
#include "Lighting.h"
#include "../Editor/ImguiManager.h"
#include "Settings.h"
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
	activeScene = CreateScene("New Scene");
#ifdef  Generate
	LoadScene(activeScene);
#else
	if (Settings::LastOpenedScenePath != "") {
		activeScene->path = Settings::LastOpenedScenePath;
		LoadScene(activeScene->path);
	} else {
		LoadScene(activeScene);
	}
#endif //  Generate
	SceneManager::inEditor = inEditor;
	if (inEditor) {
		Camera::editorCamera = std::shared_ptr<Camera>(new Camera());
		Camera::current = Camera::editorCamera;
	}
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::CreateScene(const std::string &name)
{
	return std::shared_ptr<Scene>(new Scene(name));
}

void KritiaEngine::SceneManagement::SceneManager::LoadScene(const std::shared_ptr<Scene>& scene)
{
	scene->Initialize();
	scene->path = "";
}

void KritiaEngine::SceneManagement::SceneManager::LoadScene(const std::string& path) {
	BehaviourManager::Clear();
	RendererManager::Clear();
	GameObjectManager::Clear();
	Editor::GUI::ImguiManager::currentSelectedInspectable = nullptr;
	KritiaEngine::Lighting::LightingSystem::Reset();
	activeScene = std::shared_ptr<Scene>(new Scene("", path));
	activeScene->Initialize();
}

void KritiaEngine::SceneManagement::SceneManager::SaveScene() {
	activeScene->SerializeToFile();
}

std::shared_ptr<Scene> KritiaEngine::SceneManagement::SceneManager::GetActiveScene() {
	return activeScene;
}
