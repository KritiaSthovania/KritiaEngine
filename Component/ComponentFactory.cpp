#include "ComponentFactory.h"
#include "Transform.h"
#include "../CoreModule/GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "MeshFilter.h"
#include "Collider/BoxCollider.h"
#include "Renderer/MeshRenderer.h"
using json = nlohmann::ordered_json;
using namespace KritiaEngine;
std::vector<std::string> ComponentFactory::componentType = { "Transform", "Camera", "Light", "MeshFilter", "MeshRenderer", "BoxCollider"};

void KritiaEngine::ComponentFactory::AddComponent(const json& json, GameObject* gameObject) {
	if (json["Type"] == "Transform") {
		// Do nothing
	} else if (json["Type"] == "Camera") {
		gameObject->AddComponent<Camera>();
	} else if (json["Type"] == "Light") {
		gameObject->AddComponent<Light>();
	} else if (json["Type"] == "MeshFilter") {
		gameObject->AddComponent<MeshFilter>();
	} else if (json["Type"] == "MeshRenderer") {
		gameObject->AddComponent<MeshRenderer>();
	} else if (json["Type"] == "BoxCollider") {
		gameObject->AddComponent<BoxCollider>();
	}
}

void KritiaEngine::ComponentFactory::AddComponent(const std::string& name, GameObject* gameObject) {
	if (name == "Transform") {
		// Do nothing
	} else if (name == "Camera") {
		gameObject->AddComponent<Camera>();
	} else if (name == "Light") {
		gameObject->AddComponent<Light>();
	} else if (name == "MeshFilter") {
		gameObject->AddComponent<MeshFilter>();
	} else if (name == "MeshRenderer") {
		gameObject->AddComponent<MeshRenderer>();
	} else if (name == "BoxCollider") {
		gameObject->AddComponent<BoxCollider>();
	}
}
