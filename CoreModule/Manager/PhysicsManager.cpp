#include "PhysicsManager.h"
#include "../Time.h"
#include "../../Component/Collider/Collider.h"
#include "../../Component/Transform.h"
#include <algorithm>
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
using AABBPoint = KritiaEngine::Manager::PhysicsManager::AABBPoint;
using MinMax = PhysicsManager::MinMax;

float PhysicsManager::stepSize = 0.02f;
float PhysicsManager::timer = 0;
Vector3 PhysicsManager::gravityAccelaration = Vector3(0, -9.81, 0);
std::list<Collider*> PhysicsManager::colliders = std::list<Collider*>();
std::list<DeformableBody*> PhysicsManager::deformableBodies = std::list<DeformableBody*>();
std::list<RigidBody*> PhysicsManager::rigidBodies = std::list<RigidBody*>();
std::list<Collision> PhysicsManager::collisions = std::list<Collision>();
std::list<std::pair<Collider*, Collider*>> PhysicsManager::collisionPair = std::list<std::pair<Collider*, Collider*>>();
std::vector<AABBPoint> PhysicsManager::pointsX = std::vector<AABBPoint>();
std::vector<AABBPoint> PhysicsManager::pointsY = std::vector<AABBPoint>();
std::vector<AABBPoint> PhysicsManager::pointsZ = std::vector<AABBPoint>();

// bullet
btDiscreteDynamicsWorld* PhysicsManager::dynamicsWorld;
btDefaultCollisionConfiguration* PhysicsManager::collisionConfiguration;
btCollisionDispatcher* PhysicsManager::dispatcher;
btBroadphaseInterface* PhysicsManager::broadPhase;
btSequentialImpulseConstraintSolver* PhysicsManager::solver;
btAlignedObjectArray<btCollisionShape*>* PhysicsManager::collisionShapes;

void KritiaEngine::Manager::PhysicsManager::Initialize() {
	SortAABB();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadPhase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(gravityAccelaration.x, gravityAccelaration.y, gravityAccelaration.z));
	collisionShapes = new btAlignedObjectArray<btCollisionShape*>();
}

void KritiaEngine::Manager::PhysicsManager::Clear() {
	delete dynamicsWorld;
	delete solver;
	delete broadPhase;
	delete dispatcher;
	delete collisionConfiguration;
}

void KritiaEngine::Manager::PhysicsManager::AddRigidBody(RigidBody* rigidBody) {
	rigidBodies.push_back(rigidBody);
}

void KritiaEngine::Manager::PhysicsManager::AddRigidBodyBullet(RigidBody* rigidBody) {
	dynamicsWorld->addRigidBody(rigidBody->btRB);
}

void KritiaEngine::Manager::PhysicsManager::RemoveRigidBody(RigidBody* rigidBody) {
	rigidBodies.remove(rigidBody);
	dynamicsWorld->removeRigidBody(rigidBody->btRB);
}

void KritiaEngine::Manager::PhysicsManager::AddDeformableBody(DeformableBody* deformableBody) {
	deformableBodies.push_back(deformableBody);
}

void KritiaEngine::Manager::PhysicsManager::RemoveDeformableBody(DeformableBody* deformableBody) {
	deformableBodies.remove(deformableBody);
}

void KritiaEngine::Manager::PhysicsManager::AddCollider(Collider* collider) {
	colliders.push_back(collider);
	//pointsX.push_back(AABBPoint(collider, MinMax::Min, collider->GetBound().GetMin().x));
	//pointsX.push_back(AABBPoint(collider, MinMax::Max, collider->GetBound().GetMax().x));
	//pointsY.push_back(AABBPoint(collider, MinMax::Min, collider->GetBound().GetMin().y));
	//pointsY.push_back(AABBPoint(collider, MinMax::Max, collider->GetBound().GetMax().y));
	//pointsZ.push_back(AABBPoint(collider, MinMax::Min, collider->GetBound().GetMin().z));
	//pointsZ.push_back(AABBPoint(collider, MinMax::Max, collider->GetBound().GetMax().z));

	collisionShapes->push_back(collider->collisionShape);
}

void KritiaEngine::Manager::PhysicsManager::RemoveCollider(Collider* collider) {
	colliders.remove(collider);
	// remove
}

void KritiaEngine::Manager::PhysicsManager::PhysicsUpdate() {
	if (timer > stepSize) {
		//CheckCollision();
		//ResolveCollision();
		dynamicsWorld->stepSimulation(stepSize, 10);
		for (RigidBody* rb : rigidBodies) {
			rb->PhysicsUpdate();
		}
		for (DeformableBody* db : deformableBodies) {
			db->PhysicsUpdate();
		}
	}
	collisionPair.clear();
	timer += Time::deltaTime;
}

void KritiaEngine::Manager::PhysicsManager::CheckCollision() {
	SortAABB();
	SweepAndPrune();
	CheckBVH();
}

// Sweep and Prune using AABB (collider.bound)
void KritiaEngine::Manager::PhysicsManager::SweepAndPrune() {
	std::list<AABBPoint> tempX = std::list<AABBPoint>();
	std::list<AABBPoint> tempY = std::list<AABBPoint>();
	std::list<AABBPoint> tempZ = std::list<AABBPoint>();
	std::list<std::pair<Collider*, Collider*>> collisionX = std::list<std::pair<Collider*, Collider*>>();
	std::list<std::pair<Collider*, Collider*>> collisionY = std::list<std::pair<Collider*, Collider*>>();
	std::list<std::pair<Collider*, Collider*>> collisionZ = std::list<std::pair<Collider*, Collider*>>();
	// Three point lists have the same size, so we use only one loop
	// #pragma omp parallel for
	for (int i = 0; i < pointsX.size(); i++) {
		if (pointsX[i].pos == Min) {
			tempX.push_back(pointsX[i]);
		} else {
			std::list<AABBPoint>::iterator iter = tempX.begin();
			while (iter != tempX.end()) {
				if (pointsX[i].collider == iter->collider) {
					iter = tempX.erase(iter);
				} else {
					collisionX.push_back(std::pair<Collider*, Collider*>(pointsX[i].collider, iter->collider));
					iter++;
				}
			}
		}

		if (pointsY[i].pos == Min) {
			tempY.push_back(pointsY[i]);
		} else {
			std::list<AABBPoint>::iterator iter = tempY.begin();
			while (iter != tempY.end()) {
				if (pointsY[i].collider == iter->collider) {
					iter = tempY.erase(iter);
				} else {
					collisionY.push_back(std::pair<Collider*, Collider*>(pointsY[i].collider, iter->collider));
					iter++;
				}
			}
		}

		if (pointsZ[i].pos == Min) {
			tempZ.push_back(pointsZ[i]);
		} else {
			std::list<AABBPoint>::iterator iter = tempZ.begin();
			while (iter != tempZ.end()) {
				if (pointsZ[i].collider == iter->collider) {
					iter = tempZ.erase(iter);
				} else {
					collisionZ.push_back(std::pair<Collider*, Collider*>(pointsZ[i].collider, iter->collider));
					iter++;
				}
			}
		}
	}

	// there is a collision iff it is observed on all three axes.
	for (std::pair<Collider*, Collider*> collision : collisionX) {
		std::pair<Collider*, Collider*> collisionReversed = std::pair<Collider*, Collider*>(collision.second, collision.first);
		if ((std::find(collisionY.begin(), collisionY.end(), collision) != collisionY.end() || std::find(collisionY.begin(), collisionY.end(), collisionReversed) != collisionY.end())
			&& (std::find(collisionZ.begin(), collisionZ.end(), collision) != collisionZ.end() || std::find(collisionZ.begin(), collisionZ.end(), collisionReversed) != collisionZ.end())) {
			collisionPair.push_back(collision);
		}
	}
}

void KritiaEngine::Manager::PhysicsManager::SortAABB() {
	// update value
	for (int i = 0; i < pointsX.size(); i++) {
		pointsX[i].value = pointsX[i].pos == Min ? pointsX[i].collider->GetBound().GetMin().x : pointsX[i].collider->GetBound().GetMax().x;
		pointsY[i].value = pointsY[i].pos == Min ? pointsY[i].collider->GetBound().GetMin().y : pointsY[i].collider->GetBound().GetMax().y;
		pointsZ[i].value = pointsZ[i].pos == Min ? pointsZ[i].collider->GetBound().GetMin().z : pointsZ[i].collider->GetBound().GetMax().z;
	}
	std::sort(pointsX.begin(), pointsX.end());
	std::sort(pointsY.begin(), pointsY.end());
	std::sort(pointsZ.begin(), pointsZ.end());
}

void KritiaEngine::Manager::PhysicsManager::CheckBVH() {
	for (auto collision : collisionPair) {
		Collision col1 = collision.first->CheckCollision(collision.second);
		if (col1.gameObject != nullptr) {
			collisions.push_back(col1);
		}
		Collision col2 = collision.second->CheckCollision(collision.first);
		if (col2.gameObject != nullptr) {
			collisions.push_back(col2);
		}
	}
}

void KritiaEngine::Manager::PhysicsManager::ResolveCollision() {
	for (auto collision : collisions) {
		if (collision.selfCollider->isTrigger) {
			// OnTrigerEnter ...
		} else {
			// collided object has rigidbody
			if (collision.selfCollider->gameObject->GetComponent<RigidBody>() != nullptr) {
				auto otherRB = collision.otherCollider->gameObject->GetComponent<RigidBody>();
				auto selfRB = collision.selfCollider->gameObject->GetComponent<RigidBody>();
				Vector3 impulse;
				Vector3 selfImpulse;
				if (otherRB != nullptr) {
					impulse = selfRB->mass * selfRB->GetVelocity() + otherRB->mass * otherRB->GetVelocity();
					selfImpulse = otherRB->mass / (selfRB->mass + otherRB->mass) * selfImpulse;
				} else {
					impulse = selfRB->mass * selfRB->GetVelocity();
					selfImpulse = impulse;
					selfRB->velocity = -selfRB->velocity * collision.selfCollider->bounciness;
				}
				selfImpulse *= collision.selfCollider->bounciness;
				
				for (ContactPoint p : collision.contactPoints) {
					Vector3 force1 = std::abs(Vector3::Dot(selfImpulse, p.normal)) * p.normal / PhysicsManager::stepSize;
					selfRB->AddForceAndTorque(p.position, force1 / collision.contactPoints.size() * 2);
				}
			}
		}
	}
	collisions.clear();
}
