#include "BehaviourManager.h"
#include "../../Component/MonoBehaviour.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

std::list<MonoBehaviour*> BehaviourManager::monoBehaviours = std::list<MonoBehaviour*>();
std::list<MonoBehaviour*> BehaviourManager::monoBehaviourStartList = std::list<MonoBehaviour*>();
std::list<MonoBehaviour*> BehaviourManager::monoBehaviourAwakeList = std::list<MonoBehaviour*>();
std::list<Component*> BehaviourManager::updatableComponents = std::list<Component*>();
std::list<MonoBehaviour*> BehaviourManager::monoBehaviourToDelete = std::list<MonoBehaviour*>();
std::list<Component*> BehaviourManager::updatableComponentsToDelete = std::list<Component*>();

void KritiaEngine::Manager::BehaviourManager::AddMonoBehaviour(MonoBehaviour* behaviour) {
	monoBehaviours.push_back(behaviour);
	monoBehaviourStartList.push_back(behaviour);
	// if the gameobject is inactive, add the behaviour to a awake list
	if (!behaviour->awoken) {
		monoBehaviourAwakeList.push_back(behaviour);
	}
}

void KritiaEngine::Manager::BehaviourManager::RemoveMonoBehaviour(MonoBehaviour* behaviour) {
	monoBehaviours.remove(behaviour);
	// Just in case someone added a behaviour and deleted it immidiately in the same frame
	monoBehaviourStartList.remove(behaviour);
	monoBehaviourAwakeList.remove(behaviour);
}

void KritiaEngine::Manager::BehaviourManager::AddUpdatableComponents(KritiaEngine::Component* component) {
	updatableComponents.push_back(component);
}

void KritiaEngine::Manager::BehaviourManager::RemoveUpdatableComponents(KritiaEngine::Component* component) {
	updatableComponents.remove(component);
}

void KritiaEngine::Manager::BehaviourManager::BehaviourUpdate() {
	//Try to awake behaviours first.
	if (monoBehaviourAwakeList.size() > 0) {
		for (auto iter = monoBehaviourAwakeList.begin(); iter != monoBehaviourAwakeList.end();) {
			(*iter)->BehaviourAwake();
			if ((*iter)->awoken) {
				iter = monoBehaviourAwakeList.erase(iter);
			} else {
				iter++;
			}
		}
	}
	if (monoBehaviourStartList.size() > 0) {
		for (auto iter = monoBehaviourStartList.begin(); iter != monoBehaviourStartList.end();) {
			(*iter)->BehaviourStart();
			if ((*iter)->started) {
				iter = monoBehaviourStartList.erase(iter);
			} else {
				iter++;
			}
		}
	}
	for (Component* component : updatableComponents) {
		component->ComponentUpdate();
	}
	for (MonoBehaviour* behaviour : monoBehaviours) {
		behaviour->BehaviourUpdate();
	}
}

void KritiaEngine::Manager::BehaviourManager::Clear() {
	monoBehaviours.clear();
	monoBehaviourAwakeList.clear();
	monoBehaviourStartList.clear();
	updatableComponents.clear();
}
