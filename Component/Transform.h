#pragma once
#include "Component.h"
#include "../CoreModule/GameObject.h"
#include "pch.h"

#include "../CoreModule/Mathf.h"
namespace KritiaEngine{
	class Transform : public Component {
	public:
		Transform(GameObject* gameObject);
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		Vector3 forward;
		Vector3 up;
		Vector3 right;
		std::shared_ptr<Transform> parent;
	private:
		// The rotation-ralated fields are cached.
		Vector3 cachedForward;
		Vector3 cachedUp;
		Vector3 cachedRight;
		Quaternion cachedRotation;
		void ComponentUpdate();
		std::list<std::shared_ptr<Transform>> children;
	};
}

