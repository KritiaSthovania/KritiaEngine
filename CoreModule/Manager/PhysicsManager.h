#pragma once
#include <list>
#include "../Utilities.h"

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
		static void AddCollider(Collider* collider);
		static void RemoveCollider(Collider* collider);
		static void PhysicsUpdate();
		static void Initialize();
	private:
		// Check collisions using Sweep and Prune (Collider::bound)
		static void CheckCollision();
		static void SortAABB();
		static float stepSize;
		static float timer;
		static std::list<Collider*> colliders;
		static std::list<std::tuple<Collider*, Collider*>> collisionPair;
		static std::vector<AABBPoint> pointsX;
		static std::vector<AABBPoint> pointsY;
		static std::vector<AABBPoint> pointsZ;
		
	};
}


