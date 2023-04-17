#include "BoundingVolume.h"
#include "BoxCollider.h"
#include "../RigidBody.h"
#include "../../CoreModule/Mathf.h"
using namespace KritiaEngine;

void KritiaEngine::BoundingVolumeOBB::CheckCollision(Collision* collision, BoundingVolume* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform) {
	if (dynamic_cast<BoundingVolumeOBB*>(other) != nullptr) {
		CheckCollision(collision, dynamic_cast<BoundingVolumeOBB*>(other), selfTransform, otherTransform);
	} else {
		CheckCollision(collision, dynamic_cast<BoundingVolumeOBB*>(other), selfTransform, otherTransform);
	}
}

void KritiaEngine::BoundingVolumeOBB::CheckCollision(Collision* collision, BoundingVolumeOBB* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform) {
	if (Collided(other, selfTransform, otherTransform)) {
		// reached the leaf of the BVH
		if (child1 == nullptr && child2 == nullptr) {
			collision->selfCollider = selfTransform->gameObject->GetComponent<BoxCollider>();
			collision->otherCollider = otherTransform->gameObject->GetComponent<BoxCollider>();
			collision->rigidBody = otherTransform->gameObject->GetComponent<RigidBody>();
			collision->gameObject = otherTransform->gameObject;

			// Approximation of the contact point, may need refinement
			Vector3 verticesSum = Vector3::Zero();
			Vector3 otherVerticesSum = Vector3::Zero();
			for (int i = 0; i < vertices.size(); i++) {
				verticesSum += vertices[i];
				otherVerticesSum += other->vertices[i];
			}
			Vector3 position = 1.f / 8 * verticesSum;
			Vector3 normal = Vector3::Zero();
			if (collision->selfCollider->gameObject->GetComponent<RigidBody>()!= nullptr) {
				normal = -Vector3::Normalize(collision->selfCollider->gameObject->GetComponent<RigidBody>()->GetVelocity());
			}
			ContactPoint p(normal, position, collision->selfCollider, collision->otherCollider);
			collision->contactPoints.push_back(p);
		}
		else {
			child1->CheckCollision(collision, other, selfTransform, otherTransform);
			child2->CheckCollision(collision, other, selfTransform, otherTransform);		
		}
	} 
}

float KritiaEngine::BoundingVolumeOBB::ProjectPoint(const Vector3& point, const Vector3& axis) {
	Vector3 projectedPoint = Vector3::Project(point, axis);
	return Vector3::Magnitude(projectedPoint) * Mathf::Sign(Vector3::Dot(projectedPoint, axis));
}

void KritiaEngine::BoundingVolumeOBB::GetInterval(const std::vector<Vector3>& vertices, const Vector3& axis, float* min, float* max) {
	float temp;
	*min = *max = ProjectPoint(vertices[0], axis);
	for (int i = 1; i < vertices.size(); i++) {
		temp = ProjectPoint(vertices[i], axis);
		*min = std::min(*min, temp);
		*max = std::max(*max, temp);
	}
}

bool KritiaEngine::BoundingVolumeOBB::IntervalOverlapped(BoundingVolumeOBB* other, const Vector3& axis) {
	float xMin, xMax, yMin, yMax;
	GetInterval(vertices, axis, &xMin, &xMax);
	GetInterval(other->vertices, axis, &yMin, &yMax);
	if (yMin >= xMin && yMin <= xMax) return true;
	if (yMax >= xMin && yMax <= xMax) return true;
	if (xMin >= yMin && xMin <= yMax) return true;
	if (xMax >= yMin && xMax <= yMax) return true;
	return false;
}

bool KritiaEngine::BoundingVolumeOBB::Collided(BoundingVolumeOBB* other, const std::shared_ptr<Transform>& selfTransform, const std::shared_ptr<Transform>& otherTransform) {
	bool collided = true;
	// Perform 15 tests, there is a collision iff intervals are overlapped among all 15 axes.
	collided &= IntervalOverlapped(other, selfTransform->right);
	collided &= IntervalOverlapped(other, selfTransform->up);
	collided &= IntervalOverlapped(other, selfTransform->forward);
	collided &= IntervalOverlapped(other, otherTransform->right);
	collided &= IntervalOverlapped(other, otherTransform->up);
	collided &= IntervalOverlapped(other, otherTransform->forward);

	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->right, otherTransform->right) == Vector3::Zero() ? selfTransform->right : Vector3::Cross(selfTransform->right, otherTransform->right)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->right, otherTransform->up) == Vector3::Zero() ? selfTransform->right : Vector3::Cross(selfTransform->right, otherTransform->up)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->right, otherTransform->forward) == Vector3::Zero() ? selfTransform->right : Vector3::Cross(selfTransform->right, otherTransform->forward)));

	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->up, otherTransform->right) == Vector3::Zero() ? selfTransform->up : Vector3::Cross(selfTransform->up, otherTransform->right)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->up, otherTransform->up) == Vector3::Zero() ? selfTransform->up : Vector3::Cross(selfTransform->up, otherTransform->up)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->up, otherTransform->forward) == Vector3::Zero() ? selfTransform->up : Vector3::Cross(selfTransform->up, otherTransform->forward)));

	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->forward, otherTransform->right) == Vector3::Zero() ? selfTransform->forward : Vector3::Cross(selfTransform->forward, otherTransform->right)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->forward, otherTransform->up) == Vector3::Zero() ? selfTransform->forward : Vector3::Cross(selfTransform->forward, otherTransform->up)));
	collided &= IntervalOverlapped(other, Vector3::Normalize(Vector3::Cross(selfTransform->forward, otherTransform->forward) == Vector3::Zero() ? selfTransform->forward : Vector3::Cross(selfTransform->forward, otherTransform->forward)));

	return collided;
}

void KritiaEngine::BoundingVolumeOBB::CreateChildren(int maximumDepth, int currentDepth) {
	// Vertices Order OBB: ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight. (Aligned with BoxCollider::PointPos)
	// Check the langest axis:
	if (currentDepth < maximumDepth) {
		child1 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
		child2 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
		float xLength = Vector3::Magnitude(vertices[0] - vertices[1]);
		float yLength = Vector3::Magnitude(vertices[0] - vertices[2]);
		float zLength = Vector3::Magnitude(vertices[0] - vertices[4]);
		//  length on x axis dominates, child1 left, child2 right 
		if (xLength >= yLength && xLength >= zLength) {
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
		// axis y dominates, child1 top, child2 bottom
		else if(yLength >= xLength && yLength >= zLength) {
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
		// axis z dominates, child1 forward, child2 back
		else {
			child1->vertices.push_back(vertices[0]);
			child1->vertices.push_back(vertices[1]);
			child1->vertices.push_back(vertices[2]);
			child1->vertices.push_back(vertices[3]);
			child1->vertices.push_back(0.5 * (vertices[0] + vertices[4]));
			child1->vertices.push_back(0.5 * (vertices[1] + vertices[5]));
			child1->vertices.push_back(0.5 * (vertices[2] + vertices[6]));
			child1->vertices.push_back(0.5 * (vertices[3] + vertices[7]));
			child1->CreateChildren(maximumDepth, currentDepth + 1);

			child2->vertices.push_back(0.5 * (vertices[0] + vertices[4]));
			child2->vertices.push_back(0.5 * (vertices[1] + vertices[5]));
			child2->vertices.push_back(0.5 * (vertices[2] + vertices[6]));
			child2->vertices.push_back(0.5 * (vertices[3] + vertices[7]));
			child2->vertices.push_back(vertices[4]);
			child2->vertices.push_back(vertices[5]);
			child2->vertices.push_back(vertices[6]);
			child2->vertices.push_back(vertices[7]);
			child2->CreateChildren(maximumDepth, currentDepth + 1);
		}
	}

}

void KritiaEngine::BoundingVolumeOBB::UpdateChildren(int maximumDepth, int currentDepth) {
	// Vertices Order OBB: ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight. (Aligned with BoxCollider::PointPos)
	if (currentDepth < maximumDepth) {
		if (child1 == nullptr) {
			CreateChildren(maximumDepth, currentDepth);
		}
		Vector3 diagonal = vertices[0] - vertices[7];
		float xLength = Vector3::Magnitude(vertices[0] - vertices[1]);
		float yLength = Vector3::Magnitude(vertices[0] - vertices[2]);
		float zLength = Vector3::Magnitude(vertices[0] - vertices[4]);
		//  length on x axis dominates, child1 left, child2 right 
		if (xLength >= yLength && xLength >= zLength) {
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
		// axis y dominates, child1 top, child2 bottom
		else if(yLength >= xLength && yLength >= zLength) {
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
		// axis z dominates, child1 forward, child2 back
		else {
			child1->vertices[0] = vertices[0];
			child1->vertices[1] = vertices[1];
			child1->vertices[2] = vertices[2];
			child1->vertices[3] = vertices[3];
			child1->vertices[4] = 0.5 * (vertices[0] + vertices[4]);
			child1->vertices[5] = 0.5 * (vertices[1] + vertices[5]);
			child1->vertices[6] = 0.5 * (vertices[2] + vertices[6]);
			child1->vertices[7] = 0.5 * (vertices[3] + vertices[7]);

			child2->vertices[0] = 0.5 * (vertices[0] + vertices[4]);
			child2->vertices[1] = 0.5 * (vertices[1] + vertices[5]);
			child2->vertices[2] = 0.5 * (vertices[2] + vertices[6]);
			child2->vertices[3] = 0.5 * (vertices[3] + vertices[7]);
			child2->vertices[4] = vertices[4];
			child2->vertices[5] = vertices[5];
			child2->vertices[6] = vertices[6];
			child2->vertices[7] = vertices[7];
		}
	}
	
}


