#pragma once
#include "Collider.h"
namespace KritiaEngine {
	class BoxCollider : public Collider {
	public:
		BoxCollider(GameObject* gameObject);
		// center in local space, alias for bound.center
		Vector3& Center();
		// size in local space, alias for bound.size (relative to Transform.scale) 
		Vector3& Size();
	private:
		// Í¨¹ý Collider ¼Ì³Ð
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
	};

}


