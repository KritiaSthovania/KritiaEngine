#pragma once
#ifndef COMPONENT
#define COMPONENT
#include "pch.h"
#include "../CoreModule/Object.h"

namespace KritiaEngine {
	class Transform;
	class GameObject;
	class Component : public Object {
	public:
		Component();
		Component(GameObject* gameObject);
		~Component() {
		}
		virtual void Destroy();
		/// <summary>
		/// Get the transform of this component's gameObject
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<Transform> Transform();

		// Ù–‘
		GameObject* gameObject;
	};
}

#endif



