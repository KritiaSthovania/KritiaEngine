#include "GameObjectManager.h"
#include "../Object.h"
using namespace KritiaEngine::Manager;
std::list<GameObject*> GameObjectManager::sceneGameObjects = std::list<GameObject*>();

void KritiaEngine::Manager::GameObjectManager::Clear() {
	sceneGameObjects.clear();
}
