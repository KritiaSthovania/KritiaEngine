#include "Transform.h"
#include "../CoreModule/Utilities.h"
using namespace KritiaEngine;
Transform::Transform(GameObject* gameObject) {
	this->gameObject = gameObject;
	position = Vector3(0, 0, 0);
	rotation = Quaternion::Identity();
	scale = Vector3(1, 1, 1);
	forward = Vector3(0, 0, 1);
	right = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
	rotationEuler = Quaternion::ToEuler(rotation);
	cachedPosition = position;
	cachedScale = scale;
	cachedRotation = rotation;
	cachedForward = forward;
	cachedRight = right;
	cachedUp = up;
	cachedRotationEuler = rotationEuler;
	UpdateTransformMatrix();
	rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
	KritiaEngine::Manager::BehaviourManager::AddUpdatableComponents(this);
}

void KritiaEngine::Transform::UpdateTransformMatrix() {
	transformMatrix = Matrix4x4::Identity();
	transformMatrix = Mathf::Translate(transformMatrix, position);
	transformMatrix *= Quaternion::ToRotationMatrix(rotation);
	transformMatrix = Mathf::Scale(transformMatrix, scale);
}

void KritiaEngine::Transform::ComponentUpdate() {
	// Update the rotation and three direction vectors
	if (cachedRotationEuler != rotationEuler) {
		cachedRotationEuler = rotationEuler;
		rotation = Quaternion::FromEuler(rotationEuler.x, rotationEuler.y, rotationEuler.z);
		cachedRotation = rotation;
		rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = Vector3::Normalize(rotationMatrix * Vector3(0, 1, 0));
		cachedUp = up;
		forward = Vector3::Normalize(rotationMatrix * Vector3(0, 0, 1));
		cachedForward = forward;
		right = Vector3::Normalize(rotationMatrix * Vector3(1, 0, 0));
		cachedRight = right;
		UpdateTransformMatrix();
	}else if (cachedRotation != rotation) {
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = Vector3::Normalize(rotationMatrix * Vector3(0, 1, 0));
		cachedUp = up;
		forward = Vector3::Normalize(rotationMatrix * Vector3(0, 0, 1));
		cachedForward = forward;
		right = Vector3::Normalize(rotationMatrix * Vector3(1, 0, 0));
		cachedRight = right;
		UpdateTransformMatrix();
	} else if (cachedUp != up) {
		rotation = Quaternion::FromTwoVectors(cachedUp, up);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		up = Vector3::Normalize(up);
		cachedUp = up;
		rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		forward = Vector3::Normalize(rotationMatrix * Vector3(0, 0, 1));
		cachedForward = forward;
		right = Vector3::Normalize(rotationMatrix * Vector3(1, 0, 0));
		cachedRight = right;
		UpdateTransformMatrix();
	} else if (cachedForward != forward) {
		rotation = Quaternion::FromTwoVectors(cachedForward, forward);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		forward = Vector3::Normalize(forward);
		cachedForward = forward;
		rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = Vector3::Normalize(rotationMatrix * Vector3(0, 1, 0));
		cachedUp = up;
		right = Vector3::Normalize(rotationMatrix * Vector3(1, 0, 0));
		cachedRight = right;
		UpdateTransformMatrix();
	} else if (cachedRight != right) {
		rotation = Quaternion::FromTwoVectors(cachedRight, right);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		right = Vector3::Normalize(right);
		cachedRight = right;
		rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = Vector3::Normalize(rotationMatrix * Vector3(0, 1, 0));
		cachedUp = up;
		forward = Vector3::Normalize(rotationMatrix * Vector3(0, 0, 1));
		cachedForward = forward;
		UpdateTransformMatrix();
	} else if (cachedPosition != position) {
		UpdateTransformMatrix();
		cachedPosition = position;
	} else if (cachedScale != scale) {
		UpdateTransformMatrix();
		cachedScale = scale;
	}
}

void KritiaEngine::Transform::OnInspector() {
	ImguiAlias::FloatField3("Position", &position.x);
	ImguiAlias::FloatField3("Rotation", &rotationEuler.x);
	ImguiAlias::FloatField3("Scale   ", &scale.x);
}

std::string KritiaEngine::Transform::SerializeToJson() {
	json json;
	json["Type"] = "Transform";
	json["Position"] = { position.x, position.y, position.z };
	json["Rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
	json["Scale"] = { scale.x, scale.y, scale.z };
	return json.dump();
}

void KritiaEngine::Transform::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "Transform");
	this->position = Vector3(json["Position"][0], json["Position"][1], json["Position"][2]);
	this->rotation = Quaternion(json["Rotation"][0], json["Rotation"][1], json["Rotation"][2], json["Rotation"][3]);
	this->scale = Vector3(json["Scale"][0], json["Scale"][1], json["Scale"][2]);
	UpdateTransformMatrix();
	rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
}

std::string KritiaEngine::Transform::GetInspectorLabel() {
	return inspectorLabel;
}
