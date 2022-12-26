#pragma once
#ifndef COMPONENT
#define COMPONENT
#include "pch.h"
#include "../CoreModule/Manager/BehaviourManager.h"
#include "../CoreModule/GameObject.h"
namespace KritiaEngine {
	class Transform;
	class Component : public Object {
		friend class KritiaEngine::Manager::BehaviourManager;
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
	};
}

#endif



