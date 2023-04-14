#pragma once
#include "../../CoreModule/Utilities.h"
#include "../Transform.h"
#include <memory>

namespace KritiaEngine {
	class BoundingVolume {
	public:
		std::unique_ptr<BoundingVolume> child1 = nullptr;
		std::unique_ptr<BoundingVolume> child2 = nullptr;
		// Vertices Order OBB: ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight. (Aligned with BoxCollider::PointPos)
		std::vector<Vector3> vertices = std::vector<Vector3>();
		virtual void CreateChildren(int maximumDepth, int currentDepth) = 0;
		virtual void UpdateChildren(int maximumDepth, int currentDepth) = 0;
		virtual Collision CheckCollision(Collision* collision, BoundingVolume* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform) = 0;
	};

	class BoundingVolumeOBB : public BoundingVolume {
	public:
		Collision CheckCollision(Collision* collision, BoundingVolumeOBB* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform);
		virtual void CreateChildren(int maximumDepth, int currentDepth) override;
		virtual void UpdateChildren(int maximumDepth, int currentDepth) override;
		virtual Collision CheckCollision(Collision* collision, BoundingVolume* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform) override;
	private:
		// Functions for Separating Axis Test
		float ProjectPoint(const Vector3& point, const Vector3& axis);
		void GetInterval(const std::vector<Vector3>& vertices, const Vector3& axis, float* min, float* max);
		bool IntervalOverlapped(BoundingVolumeOBB* other, const Vector3& axis);
		bool Collided(BoundingVolumeOBB* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform);
	};
}

