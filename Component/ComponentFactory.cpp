#include "ComponentFactory.h"
#include "Transform.h"
using json = nlohmann::ordered_json;

void KritiaEngine::ComponentFactory::AddComponentFromJson(const json& json, GameObject* gameObject) {
	if (json["Type"] == "Transform") {
		
	}
}
