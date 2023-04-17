#pragma once
#include <list>
#include "../Utilities.h"
#include "../../Component/RigidBody.h"
namespace KritiaEngine {
	class Collider;
}

namespace KritiaEngine::Manager {
	class PhysicsManager {
	public:
		enum MinMax {
			Min = 0,
			Max = 1
		};

		struct AABBPoint {
			Collider* collider;
			MinMax pos;
			float value;
			bool operator<(const AABBPoint& other) const {
				return value < other.value;
			}
		};
		static void AddRigidBody(RigidBody* rigidBody);
		static void RemoveRigidBody(RigidBody* rigidBody);
		static void AddCollider(Collider* collider);
		static void RemoveCollider(Collider* collider);
		static void PhysicsUpdate();
		static void Initialize();
		static Vector3 gravityAccelaration;
		static float stepSize;
	private:
		// Check collisions using Sweep and Prune (Collider::bound)
		static void CheckCollision();
		static void SweepAndPrune();
		static void SortAABB();
		static void CheckBVH();
		static void ResolveCollision();
		static float timer;
		static std::list<RigidBody*> rigidBodies;
		static std::list<Collider*> colliders;
		static std::list<std::pair<Collider*, Collider*>> collisionPair;
		static std::list<Collision> collisions;
		static std::vector<AABBPoint> pointsX;
		static std::vector<AABBPoint> pointsY;
		static std::vector<AABBPoint> pointsZ;

	};
}


