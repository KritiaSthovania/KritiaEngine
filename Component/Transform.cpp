#include "Transform.h"

using namespace KritiaEngine;
Transform::Transform(GameObject* gameObject) {
	this->gameObject = gameObject;
	Position = Vector3(0, 0, 0);
	Rotation = Quaternion::Identity();
	Scale = Vector3(1, 1, 1);
}