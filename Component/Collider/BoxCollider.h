#pragma once
#include "Collider.h"
#include "BoundingVolume.h"
namespace KritiaEngine::Manager {
	class PhysicsManager;
}

namespace KritiaEngine {
	class BoundingVolumeOBB;
	/// <summary>
	/// An Oriented-Bounding-Box (OBB), aligned with the Transform.
	/// </summary>
	class BoxCollider : public Collider {
		friend class KritiaEngine::Manager::PhysicsManager;
	public:
		BoxCollider(GameObject* gameObject);
		// center in local space, alias for bound.center
		Vector3 center;
		// size in local space, alias for bound.size (relative to Transform.scale) 
		Vector3 size;
		int boundingVolumeDepth = 3;
	private:
		enum PointPos {
			ForwardBottomLeft,
			ForwardBottomRight,
			ForwardTopLeft,
			ForwardTopRight,
			BackBottomLeft,
			BackBottomRight,
			BackTopLeft,
			BackTopRight
		};
		int cachedBoundingVolumeDepth = 3;
		Vector3 cachedPosition = Vector3::Zero();
		Quaternion cachedRotation = Quaternion::Identity();
		Vector3 cachedScale = Vector3(1, 1, 1);
		// Get the points in world space
		Vector3 GetVertex(PointPos pos);
		// Í¨¹ý Collider ¼Ì³Ð
		virtual void ComponentUpdate() override;
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		virtual void UpdateBound() override;
		virtual void UpdateBoundingVolume() override;
	};

}


