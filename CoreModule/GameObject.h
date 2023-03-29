#pragma once

#include "pch.h"
#include "Interface/ISerializable.h"
#include "Interface/IInspectable.h"
#include "Object.h"
#include <typeinfo>
using json = nlohmann::ordered_json;

namespace KritiaEngine::Editor::GUI {
	class InspectorWindow;
	class HierachyWindow;
	class ProjectFileExplorer;
}

namespace KritiaEngine::SceneManagement {
	class Scene;
}

namespace KritiaEngine::Manager {
	class ResourceManager;
}

namespace KritiaEngine {
	class Transform;
	class Component;
	class GameObject : public Object, IJsonSerializable, IFileSerializable, IInspectable{
		friend class KritiaEngine::Editor::GUI::InspectorWindow;
		friend class KritiaEngine::Editor::GUI::HierachyWindow;
		friend class KritiaEngine::Editor::GUI::ProjectFileExplorer;
		friend class KritiaEngine::Manager::ResourceManager;
		friend class KritiaEngine::SceneManagement::Scene;
		friend class KritiaEngine::Component;
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
		bool isActive = true;
	private:
		std::list<std::shared_ptr<Component>> components;
		std::vector<std::shared_ptr<Component>> componentToDelete;
		std::string path = "";
		bool hasPrefab = false;
		/// <summary>
		/// Serialize the GameObject as part of a scene.
		/// </summary>
		/// <param name="json">Json of the scene</param>
		virtual std::string SerializeToJson() override;
		static GameObject* DeserializeFromJson(const json& json);
		static GameObject* DeserializeFromPath(const std::string& path);
		static GameObject* DeserializeAsPrefab(const json& json);
		void AddComponentFromJson(const json& json);
		void ShowAddComponentPopup();
		// 通过 FileSerializable 继承
		virtual void SerializeToFile() override;
		virtual void OnObjectDestroy() override;

		// 通过 Inspectable 继承
		virtual void OnInspector() override;
	};
}


