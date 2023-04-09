#include "PhysicsManager.h"
#include "../../Component/Collider/Collider.h"
#include "../Time.h"
#include <algorithm>
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
using AABBPoint = KritiaEngine::Manager::PhysicsManager::AABBPoint;
using MinMax = PhysicsManager::MinMax;

float PhysicsManager::stepSize = 0.03f;
float PhysicsManager::timer = 0;
std::list<Collider*> PhysicsManager::colliders = std::list<Collider*>();
std::list<std::tuple<Collider*, Collider*>> PhysicsManager::collisionPair = std::list<std::tuple<Collider*, Collider*>>();
std::vector<AABBPoint> PhysicsManager::pointsX = std::vector<AABBPoint>();
std::vector<AABBPoint> PhysicsManager::pointsY = std::vector<AABBPoint>();
std::vector<AABBPoint> PhysicsManager::pointsZ = std::vector<AABBPoint>();

void KritiaEngine::Manager::PhysicsManager::AddCollider(Collider* collider) {
		colliders.push_back(collider);
		pointsX.push_back(AABBPoint(collider, MinMax::Min, collider->bound.GetMin().x));
		pointsX.push_back(AABBPoint(collider, MinMax::Max, collider->bound.GetMax().x));
		pointsY.push_back(AABBPoint(collider, MinMax::Min, collider->bound.GetMin().y));
		pointsY.push_back(AABBPoint(collider, MinMax::Max, collider->bound.GetMax().y));
		pointsZ.push_back(AABBPoint(collider, MinMax::Min, collider->bound.GetMin().z));
		pointsZ.push_back(AABBPoint(collider, MinMax::Max, collider->bound.GetMax().z));
}

void KritiaEngine::Manager::PhysicsManager::RemoveCollider(Collider* collider) {
	colliders.remove(collider);
	// remove
}

void KritiaEngine::Manager::PhysicsManager::PhysicsUpdate() {
	if (timer > stepSize) {

	}
	collisionPair.clear();
	timer += Time::deltaTime;
}

void KritiaEngine::Manager::PhysicsManager::Initialize() {
	SortAABB();
}

// Sweep and Prune using AABB
void KritiaEngine::Manager::PhysicsManager::CheckCollision() {
	SortAABB();
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
		if (std::find(collisionY.begin(), collisionY.end(), collision) != collisionY.end() && std::find(collisionZ.begin(), collisionZ.end(), collision) != collisionZ.end()) {
			collisionPair.push_back(collision);
		}
	}
}

void KritiaEngine::Manager::PhysicsManager::SortAABB() {
	std::sort(pointsX.begin(), pointsX.end());
	std::sort(pointsY.begin(), pointsY.end());
	std::sort(pointsZ.begin(), pointsZ.end());
}
