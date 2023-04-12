#pragma once
#include "Collider.h"

namespace KritiaEngine::Manager {
	class PhysicsManager;
}

namespace KritiaEngine {
	/// <summary>
	/// An Oriented-Bounding-Box (OBB), aligned with the Transform.
	/// </summary>
	class BoxCollider : public Collider {
		friend class KritiaEngine::Manager::PhysicsManager;
	public:
		enum PointPos {
			ForwardBottomLeft = 0,
			ForwardBottomRight,
			ForwardTopLeft,
			ForwardTopRight,
			BackBottomLeft,
			BackBottomRight,
			BackTopLeft,
			BackTopRight
		};
		BoxCollider(GameObject* gameObject);
		// Get the points in world space
		Vector3 GetExtremePoint(PointPos pos);
		// center in local space, alias for bound.center
		Vector3 center;
		// size in local space, alias for bound.size (relative to Transform.scale) 
		Vector3 size;
	private:
		// 通过 Collider 继承
		virtual void ComponentUpdate() override;
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;

		// 通过 Collider 继承
		virtual void UpdateBound() override;
	};

}


