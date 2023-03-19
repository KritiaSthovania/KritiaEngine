#pragma once
#include "../GameObject.h"
#include <list>
using namespace KritiaEngine;
namespace KritiaEngine::Manager {
	class GameObjectManager {
	public:
		static void Clear();
		static std::list<GameObject*> sceneGameObjects;
	};
}


