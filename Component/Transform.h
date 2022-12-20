#pragma once
#include "Component.h"
#include "../CoreModule/GameObject.h"
#include "pch.h"

#include "../CoreModule/Mathf.h"
namespace KritiaEngine{
	class Transform : public Component {
	public:
		Transform(GameObject* gameObject);
		~Transform() {
		}
		Vector3 Position;
		Vector3 Scale;
		Quaternion Rotation;
		std::shared_ptr<Transform> parent;
	private:
		std::list<std::shared_ptr<Transform>> children;
	};
}

