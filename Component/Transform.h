#pragma once
#include "Component.h"
#include "../CoreModule/GameObject.h"
#include "pch.h"
#include "../CoreModule/Mathf.h"

using json = nlohmann::ordered_json;
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
		Vector3 rotationEuler;
	private:
		// The rotation-ralated fields are cached.
		Vector3 cachedForward;
		Vector3 cachedUp;
		Vector3 cachedRight;
		Quaternion cachedRotation;

		Vector3 cachedRotationEuler;
		void ComponentUpdate();
		std::list<std::shared_ptr<Transform>> children;
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "Transform";
	};
}

