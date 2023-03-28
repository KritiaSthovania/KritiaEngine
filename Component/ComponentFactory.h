#pragma once
#include "Component.h"

using json = nlohmann::ordered_json;

namespace KritiaEngine {
	class ComponentFactory {
		friend class GameObject;
		static std::vector<std::string> componentType;
	private:
		static void AddComponent(const json& json, GameObject* gameObject);
		static void AddComponent(const std::string& name, GameObject* gameObject);
	};
}


