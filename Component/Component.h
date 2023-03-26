#pragma once
#ifndef COMPONENT
#define COMPONENT
#include "pch.h"
#include "../CoreModule/Interface/ISerializable.h"
#include "../CoreModule/Interface/IInspectable.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/GameObject.h"
#include "../Editor/ImguiAlias.h"
#include "../Editor/ImguiManager.h"
#include <imgui/imgui.h>
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;

namespace KritiaEngine {
	class Transform;
	class Component : public Object, private IJsonSerializable, IJsonDeserializable, IInspectable{
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
	protected:
		virtual void OnObjectDestroy() override;
	private:
		// If this component is attached to BehaviourManager, this update function will always be called.
		// Must manually add this component to BehaviourManager
		virtual void ComponentUpdate();
		virtual void OnInspector() override = 0;
		virtual std::string SerializeToJson() override = 0;
		virtual void DeserializeFromJson(const json& json) override = 0;
		virtual std::string GetInspectorLabel () = 0;
		std::string inspectorLabel;
	};
}

#endif



