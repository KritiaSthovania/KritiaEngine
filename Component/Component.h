#pragma once
#ifndef COMPONENT
#define COMPONENT
#include <imgui/imgui.h>
#include "pch.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/GameObject.h"

namespace KritiaEngine {
	class Transform;
	class Component : public Object {
		friend class KritiaEngine::Manager::BehaviourManager;
		friend class KritiaEngine::Editor::GUI::InspectorWindow;
		friend class GameObject;
	public:
		Component();
		Component(GameObject* gameObject);
		/// <summary>
		/// Get the transform of this component's gameObject
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<Transform> Transform();

		// Ù–‘
		GameObject* gameObject;
	private:
		// If this component is attached to BehaviourManager, this update function will always be called.
		// Must manually add this component to BehaviourManager
		virtual void ComponentUpdate();
		virtual void OnInspector();
		/// <summary>
		/// Serialize the information of a component to a GameObject
		/// </summary>
		/// <param name="json">json of the GameObject</param>
		/// <param name="componentIndex">index of the component, start from 0</param>
		virtual void ComponentSerialize(nlohmann::json& json, int componentIndex);
	};
}

#endif



