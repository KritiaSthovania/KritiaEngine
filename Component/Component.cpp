#include "Component.h"
#include "../CoreModule/GameObject.h"
using namespace KritiaEngine;
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

void KritiaEngine::Component::ComponentUpdate() {}

void KritiaEngine::Component::OnInspector() {}

std::string KritiaEngine::Component::Serialize() {
	return "Virtual serialization function of Component is called ";
}

void KritiaEngine::Component::Deserialize(const json& json) {

}

