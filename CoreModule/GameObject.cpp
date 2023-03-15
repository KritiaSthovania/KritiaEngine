#include "GameObject.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "../Component/Component.h"
#include "../Component/ComponentFactory.h"
using namespace KritiaEngine;
using namespace nlohmann::literals;
using json = nlohmann::ordered_json;

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

std::string KritiaEngine::GameObject::SerializeToJson() {
	json json;
	json["Type"] = "GameObject";
	json["Name"] = this->name;
	json["Number Of Components"] = this->components.size();
	int componentIndex = 0;
	for (std::shared_ptr<Component> comp : components) {
		json["Component" + std::to_string(componentIndex)] = comp->SerializeToJson();
		componentIndex++;
	}
	return json.dump();
}

void KritiaEngine::GameObject::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "GameObject");
	this->name = json["Name"];
	int numberOfComponents = json["Number Of Components"];
	int componentIndex = 0;
	for (int i = 0; i < numberOfComponents; i++) {
		nlohmann::ordered_json componentJson = json::parse((std::string)json["Component" + std::to_string(i)]);
		this->AddComponentFromJson(componentJson);
		(*this->components.rbegin())->DeserializeFromJson(componentJson);
	}
}

void KritiaEngine::GameObject::AddComponentFromJson(const json& json) {
	ComponentFactory::AddComponentFromJson(json, this);
}





