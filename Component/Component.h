#pragma once
#ifndef COMPONENT
#define COMPONENT
#include <imgui/imgui.h>
#include "pch.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/GameObject.h"
#include "../Editor/ImguiAlias.h"
#include "../Editor/ImguiManager.h"
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;

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
		virtual void OnInspector() = 0;
		virtual std::string Serialize() = 0;
		virtual void Deserialize(const json& json) = 0;
		virtual std::string GetInspectorLabel () = 0;
		std::string inspectorLabel;
	};
}

#endif



