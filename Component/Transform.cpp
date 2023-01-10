#include "Transform.h"

using namespace KritiaEngine;
Transform::Transform(GameObject* gameObject) {
	this->gameObject = gameObject;
	position = Vector3(0, 0, 0);
	rotation = Quaternion::Identity();
	scale = Vector3(1, 1, 1);
	forward = Vector3(0, 0, 1);
	right = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
	cachedRotation = rotation;
	cachedForward = forward;
	cachedRight = right;
	cachedUp = up;
	KritiaEngine::Manager::BehaviourManager::AddUpdatableComponents(this);
}

void KritiaEngine::Transform::ComponentUpdate() {
	// Update the rotation and three direction vectors
	if (cachedRotation != rotation) {
		cachedRotation = rotation;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * up;
		cachedUp = up;
		forward = rotationMatrix * forward;
		cachedForward = forward;
		right = rotationMatrix * right;
		cachedRight = right;
	} else if (cachedUp != up) {
		rotation = Quaternion::FromTwoVectors(cachedUp, up);
		cachedRotation = rotation;
		up = Vector3::Normalize(up);
		cachedUp = up;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		forward = rotationMatrix * forward;
		cachedForward = forward;
		right = rotationMatrix * right;
		cachedRight = right;
	} else if (cachedForward != forward) {
		rotation = Quaternion::FromTwoVectors(cachedForward, forward);
		cachedRotation = rotation;
		forward = Vector3::Normalize(forward);
		cachedForward = forward;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * up;
		cachedUp = up;
		right = rotationMatrix * right;
		cachedRight = right;
	} else if (cachedRight != right) {
		rotation = Quaternion::FromTwoVectors(cachedRight, right);
		cachedRotation = rotation;
		right = Vector3::Normalize(right);
		cachedRight = right;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * up;
		cachedUp = up;
		forward = rotationMatrix * forward;
		cachedForward = forward;
	}
}
