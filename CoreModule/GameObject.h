#pragma once

#include "pch.h"
#include <typeinfo>
#include "Object.h"

using json = nlohmann::json;

namespace KritiaEngine::Editor::GUI {
	class InspectorWindow;
}

namespace KritiaEngine {
	class Transform;
	class Component;
	class GameObject : public Object{
		friend class KritiaEngine::Editor::GUI::InspectorWindow;
	public:
		GameObject();
		GameObject(const char* name);
		std::shared_ptr<Transform> Transform();
		template <typename T> std::shared_ptr<T> GetComponent() {
			for (std::list<std::shared_ptr<Component>>::iterator iter = components.begin(); iter != components.end(); iter++) {
				if (typeid(*(*iter).get()) == typeid(T)) {
					std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(*iter);
					return component;
				}
			}
			return nullptr;
		}
		template <typename T> std::shared_ptr<T> AddComponent() {
			std::shared_ptr<T> component = std::shared_ptr<T>(new T(this));
			components.push_back(component);
			return component;
		}
		void SetActive(bool isActive);
		bool IsActive = true;
	private:
		std::list<std::shared_ptr<Component>> components;
		virtual void Serialize();
	};
}


