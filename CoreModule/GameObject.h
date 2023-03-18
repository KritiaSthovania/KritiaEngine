#pragma once

#include "pch.h"
#include "Interface/SerializableAndDeserializable.h"
#include "Object.h"
#include <typeinfo>
using json = nlohmann::ordered_json;

namespace KritiaEngine::Editor::GUI {
	class InspectorWindow;
}

namespace KritiaEngine::SceneManagement {
	class Scene;
}

namespace KritiaEngine {
	class Transform;
	class Component;
	class GameObject : public Object, JsonSerializable, JsonDeserializable, FileSerializable{
		friend class KritiaEngine::Editor::GUI::InspectorWindow;
		friend class KritiaEngine::SceneManagement::Scene;
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
		/// <summary>
		/// Serialize the GameObject as part of a scene.
		/// </summary>
		/// <param name="json">Json of the scene</param>
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		void AddComponentFromJson(const json& json);

		// Í¨¹ý FileSerializable ¼Ì³Ð
		virtual void SerializeToFile() override;
	};
}


