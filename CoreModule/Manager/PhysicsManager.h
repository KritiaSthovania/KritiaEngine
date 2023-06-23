#pragma once
#include <list>
#include "../Utilities.h"
#include "../../Component/RigidBody.h"
#include "../../Component/DeformableBody.h"
#include <bullet/btBulletDynamicsCommon.h>
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
		static void Initialize();
		static void AddRigidBody(RigidBody* rigidBody);
		static void AddRigidBodyBullet(RigidBody* rigidBody);
		static void RemoveRigidBody(RigidBody* rigidBody);
		static void AddDeformableBody(DeformableBody* deformableBody);
		static void RemoveDeformableBody(DeformableBody* deformableBody);
		static void AddCollider(Collider* collider);
		static void RemoveCollider(Collider* collider);
		static void PhysicsUpdate();
		static void Clear();
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
		static std::list<DeformableBody*> deformableBodies;
		static std::list<RigidBody*> rigidBodies;
		static std::list<Collider*> colliders;
		static std::list<std::pair<Collider*, Collider*>> collisionPair;
		static std::list<Collision> collisions;
		static std::vector<AABBPoint> pointsX;
		static std::vector<AABBPoint> pointsY;
		static std::vector<AABBPoint> pointsZ;

		//bullet
		static btAlignedObjectArray<btCollisionShape*>* collisionShapes;
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		static btDefaultCollisionConfiguration* collisionConfiguration;
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		static btBroadphaseInterface* broadPhase;
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		static btCollisionDispatcher* dispatcher;
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	    static btSequentialImpulseConstraintSolver* solver;
		static btDiscreteDynamicsWorld* dynamicsWorld;
	};
}


