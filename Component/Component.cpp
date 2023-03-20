#include "Component.h"
#include "../CoreModule/GameObject.h"
#include "../CoreModule/Manager/BehaviourManager.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
Component::Component()
{
	gameObject = nullptr;
}
KritiaEngine::Component::Component(GameObject* gameObject)
{
	this->gameObject = gameObject;
	this->name = gameObject->name;
}

std::shared_ptr<Transform> KritiaEngine::Component::Transform() {
	return this->gameObject->Transform();
}

void KritiaEngine::Component::OnObjectDestroy() {
	BehaviourManager::RemoveUpdatableComponents(this);
}

void KritiaEngine::Component::ComponentUpdate() {}

