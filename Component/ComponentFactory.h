#pragma once
#include "Component.h"

using json = nlohmann::ordered_json;

namespace KritiaEngine {
	class ComponentFactory {
		friend class GameObject;
	private:
		static void AddComponentFromJson(const json& json, GameObject* gameObject);
	};
}


