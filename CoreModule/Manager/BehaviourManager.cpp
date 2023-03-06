#include "BehaviourManager.h"
#include "../../Component/MonoBehaviour.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

std::list<MonoBehaviour*> BehaviourManager::monoBehaviours = std::list<MonoBehaviour*>();
std::list<MonoBehaviour*> BehaviourManager::monoBehaviourStartQueue = std::list<MonoBehaviour*>();
std::list<MonoBehaviour*> BehaviourManager::monoBehaviourAwakeQueue = std::list<MonoBehaviour*>();
std::list<Component*> BehaviourManager::updatableComponents = std::list<Component*>();


void KritiaEngine::Manager::BehaviourManager::AddMonoBehaviour(MonoBehaviour* behaviour) {
	monoBehaviours.push_back(behaviour);
	monoBehaviourStartQueue.push_back(behaviour);
	// if the gameobject is inactive, add the behaviour to a awake list
	if (!behaviour->awoken) {
		monoBehaviourAwakeQueue.push_back(behaviour);
	}
}

void KritiaEngine::Manager::BehaviourManager::RemoveMonoBehaviour(MonoBehaviour* behaviour) {
	monoBehaviours.remove(behaviour);
	// Just in case someone added a behaviour and deleted it immidiately in the same frame
	monoBehaviourStartQueue.remove(behaviour);
	monoBehaviourAwakeQueue.remove(behaviour);
}

void KritiaEngine::Manager::BehaviourManager::AddUpdatableComponents(KritiaEngine::Component* component) {
	updatableComponents.push_back(component);
}

void KritiaEngine::Manager::BehaviourManager::RemoveUpdatableComponents(KritiaEngine::Component* component) {
	updatableComponents.remove(component);
}

void KritiaEngine::Manager::BehaviourManager::BehaviourUpdate() {
	//Try to awake behaviours first.
	if (monoBehaviourAwakeQueue.size() > 0) {
		for (auto iter = monoBehaviourAwakeQueue.begin(); iter != monoBehaviourAwakeQueue.end();) {
			(*iter)->BehaviourAwake();
			if ((*iter)->awoken) {
				iter = monoBehaviourAwakeQueue.erase(iter);
			} else {
				iter++;
			}
		}
	}
	if (monoBehaviourStartQueue.size() > 0) {
		for (auto iter = monoBehaviourStartQueue.begin(); iter != monoBehaviourStartQueue.end();) {
			(*iter)->BehaviourStart();
			if ((*iter)->started) {
				iter = monoBehaviourStartQueue.erase(iter);
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
	monoBehaviourAwakeQueue.clear();
	monoBehaviourStartQueue.clear();
	updatableComponents.clear();
}
