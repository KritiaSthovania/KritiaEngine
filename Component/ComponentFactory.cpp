#include "ComponentFactory.h"
#include "Transform.h"
#include "../CoreModule/GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "MeshFilter.h"
#include "Renderer/MeshRenderer.h"
using json = nlohmann::ordered_json;

void KritiaEngine::ComponentFactory::AddComponentFromJson(const json& json, GameObject* gameObject) {
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
	}
}
