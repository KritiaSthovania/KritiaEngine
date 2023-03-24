#include "MonoBehaviour.h"
#include "../CoreModule/GameObject.h"
#include "../CoreModule/Manager/BehaviourManager.h"
using namespace KritiaEngine::Manager;

KritiaEngine::MonoBehaviour::MonoBehaviour(GameObject* gameObject) {
	this->gameObject = gameObject;
	BehaviourAwake();
	BehaviourManager::AddMonoBehaviour(this);
}

void  KritiaEngine::MonoBehaviour::OnObjectDestroy() {
	BehaviourManager::RemoveMonoBehaviour(this);
	OnDestroy();
}

void KritiaEngine::MonoBehaviour::BehaviourAwake() {
	if (gameObject->isActive && !awoken) {
		Awake();
		awoken = true;
	}
}

void KritiaEngine::MonoBehaviour::BehaviourStart() {
	if (gameObject->isActive && enabled && !started) {
		Start();
		started = true;
	}
}

void KritiaEngine::MonoBehaviour::BehaviourUpdate() {
	if (gameObject->isActive && enabled) {
		Update();
	}
}

void KritiaEngine::MonoBehaviour::OnInspector() {}

std::string KritiaEngine::MonoBehaviour::SerializeToJson() {
	return std::string();
}

void KritiaEngine::MonoBehaviour::DeserializeFromJson(const json& json) {}

std::string KritiaEngine::MonoBehaviour::GetInspectorLabel() {
	return inspectorLabel;
}
