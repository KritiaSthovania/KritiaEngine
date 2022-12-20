#pragma once

#include "pch.h"
#include <typeinfo>
#include "../Component/Component.h"
#include "../Component/Camera.h"
#include "Object.h"

namespace KritiaEngine {
	class Transform;
	class GameObject : public Object{
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
		void Render(std::shared_ptr<Camera> camera);
		void SetActive(bool isActive);
		bool IsActive = true;
	private:
		std::list<std::shared_ptr<Component>> components;
	};
}


