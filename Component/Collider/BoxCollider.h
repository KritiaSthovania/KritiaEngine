#pragma once
#include "Collider.h"

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
		std::unique_ptr<BoundingVolumeOBB> boundingVolume1 = nullptr;
		std::unique_ptr<BoundingVolumeOBB> boundingVolume2 = nullptr;
		Vector3 cachedPosition = Vector3::Zero();
		Quaternion cachedRotation = Quaternion::Identity();
		Vector3 cachedScale = Vector3(1, 1, 1);
		// Get the points in world space
		Vector3 GetVertex(PointPos pos);
		// 通过 Collider 继承
		virtual void ComponentUpdate() override;
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		// 通过 Collider 继承
		virtual void UpdateBound() override;
		virtual void UpdateBoundingVolume() override;
	};




	class BoundingVolumeOBB {
		friend class KritiaEngine::Manager::PhysicsManager;
		friend class KritiaEngine::BoxCollider;
	private:
		std::unique_ptr<BoundingVolumeOBB> child1 = nullptr;
		std::unique_ptr<BoundingVolumeOBB> child2 = nullptr;
		// Order:  ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight. (Aligned with BoxCollider::PointPos)
		std::vector<Vector3> vertices = std::vector<Vector3>();
		void CreateChildren(int maximumDepth, int currentDepth) {
			if (currentDepth < maximumDepth) {
				child1 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
				child2 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
				// even, child1 top, child2 bottom 
				if (currentDepth % 2 == 0) {
					child1->vertices.push_back(0.5 * (vertices[0] + vertices[2]));
					child1->vertices.push_back(0.5 * (vertices[1] + vertices[3]));
					child1->vertices.push_back(vertices[2]);
					child1->vertices.push_back(vertices[3]);
					child1->vertices.push_back(0.5 * (vertices[4] + vertices[6]));
					child1->vertices.push_back(0.5 * (vertices[5] + vertices[7]));
					child1->vertices.push_back(vertices[6]);
					child1->vertices.push_back(vertices[7]);
					child1->CreateChildren(maximumDepth, currentDepth + 1);

					child2->vertices.push_back(vertices[0]);
					child2->vertices.push_back(vertices[1]);
					child2->vertices.push_back(0.5 * (vertices[0] + vertices[2]));
					child2->vertices.push_back(0.5 * (vertices[1] + vertices[3]));
					child2->vertices.push_back(vertices[4]);
					child2->vertices.push_back(vertices[5]);
					child2->vertices.push_back(0.5 * (vertices[4] + vertices[6]));
					child2->vertices.push_back(0.5 * (vertices[5] + vertices[7]));
					child2->CreateChildren(maximumDepth, currentDepth + 1);
				}
				// odd, child1 left, child2 right
				else {
					child1->vertices.push_back(vertices[0]);
					child1->vertices.push_back(0.5 * (vertices[0] + vertices[1]));
					child1->vertices.push_back(vertices[2]);
					child1->vertices.push_back(0.5 * (vertices[2] + vertices[3]));
					child1->vertices.push_back(vertices[4]);
					child1->vertices.push_back(0.5 * (vertices[4] + vertices[5]));
					child1->vertices.push_back(vertices[6]);
					child1->vertices.push_back(0.5 * (vertices[6] + vertices[7]));
					child1->CreateChildren(maximumDepth, currentDepth + 1);

					child2->vertices.push_back(0.5 * (vertices[0] + vertices[1]));
					child2->vertices.push_back(vertices[1]);
					child2->vertices.push_back(0.5 * (vertices[2] + vertices[3]));
					child2->vertices.push_back(vertices[3]);
					child2->vertices.push_back(0.5 * (vertices[4] + vertices[5]));
					child2->vertices.push_back(vertices[5]);
					child2->vertices.push_back(0.5 * (vertices[6] + vertices[7]));
					child2->vertices.push_back(vertices[7]);
					child2->CreateChildren(maximumDepth, currentDepth + 1);
				}
			}
			
		}
		void UpdateChildren(int maximumDepth, int currentDepth) {
			if (currentDepth < maximumDepth) {
				// even, child1 top, child2 bottom 
				if (currentDepth % 2 == 0) {
					child1->vertices[0] = 0.5 * (vertices[0] + vertices[2]);
					child1->vertices[1] = 0.5 * (vertices[1] + vertices[3]);
					child1->vertices[2] = vertices[2];
					child1->vertices[3] = vertices[3];
					child1->vertices[4] = 0.5 * (vertices[4] + vertices[6]);
					child1->vertices[5] = 0.5 * (vertices[5] + vertices[7]);
					child1->vertices[6] = vertices[6];
					child1->vertices[7] = vertices[7];
					child1->UpdateChildren(maximumDepth, currentDepth + 1);

					child2->vertices[0] = vertices[0];
					child2->vertices[1] = vertices[1];
					child2->vertices[2] = 0.5 * (vertices[0] + vertices[2]);
					child2->vertices[3] = 0.5 * (vertices[1] + vertices[3]);
					child2->vertices[4] = vertices[4];
					child2->vertices[5] = vertices[5];
					child2->vertices[6] = 0.5 * (vertices[4] + vertices[6]);
					child2->vertices[7] = 0.5 * (vertices[5] + vertices[7]);
					child2->UpdateChildren(maximumDepth, currentDepth + 1);
				}
				// odd, child1 left, child2 right
				else {
					child1->vertices[0] = vertices[0];
					child1->vertices[1] = 0.5 * (vertices[0] + vertices[1]);
					child1->vertices[2] = vertices[2];
					child1->vertices[3] = 0.5 * (vertices[2] + vertices[3]);
					child1->vertices[4] = vertices[4];
					child1->vertices[5] = 0.5 * (vertices[4] + vertices[5]);
					child1->vertices[6] = vertices[6];
					child1->vertices[7] = 0.5 * (vertices[6] + vertices[7]);
					child1->UpdateChildren(maximumDepth, currentDepth + 1);

					child2->vertices[0] = 0.5 * (vertices[0] + vertices[1]);
					child2->vertices[1] = vertices[1];
					child2->vertices[2] = 0.5 * (vertices[2] + vertices[3]);
					child2->vertices[3] = vertices[3];
					child2->vertices[4] = 0.5 * (vertices[4] + vertices[5]);
					child2->vertices[5] = vertices[5];
					child2->vertices[6] = 0.5 * (vertices[6] + vertices[7]);
					child2->vertices[7] = vertices[7];
					child2->UpdateChildren(maximumDepth, currentDepth + 1);
				}
			}
		}
	};
}


