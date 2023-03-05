#include "GameObject.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "../Component/Component.h"

using namespace KritiaEngine;
using json = nlohmann::json;

KritiaEngine::GameObject::GameObject() {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = "New Game Object";

}

KritiaEngine::GameObject::GameObject(const char* name) {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = name;
}

std::shared_ptr<KritiaEngine::Transform> KritiaEngine::GameObject::Transform()
{
	return this->GetComponent<KritiaEngine::Transform>();
}

void KritiaEngine::GameObject::GameObject::SetActive(bool isActive)
{
	IsActive = isActive;
}

void KritiaEngine::GameObject::Serialize() {
	json json;
	json["Type"] = "GameObject";
	json["Name"] = this->name;
	json["Number Of Components"] = this->components.size();
	int componentIndex = 0;
	for (std::shared_ptr<Component> comp : components) {
		comp->ComponentSerialize(json, componentIndex);
		componentIndex++;
	}
	std::cout << json << std::endl;
}





