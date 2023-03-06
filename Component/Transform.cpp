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
	rotationEuler = Quaternion::ToEuler(rotation);
	cachedRotation = rotation;
	cachedForward = forward;
	cachedRight = right;
	cachedUp = up;
	cachedRotationEuler = rotationEuler;
	KritiaEngine::Manager::BehaviourManager::AddUpdatableComponents(this);
}

void KritiaEngine::Transform::ComponentUpdate() {
	// Update the rotation and three direction vectors
	if (cachedRotationEuler != rotationEuler) {
		cachedRotationEuler = rotationEuler;
		rotation = Quaternion::FromEuler(rotationEuler.x, rotationEuler.y, rotationEuler.z);
		cachedRotation = rotation;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * Vector3(0, 1, 0);
		cachedUp = up;
		forward = rotationMatrix * Vector3(0, 0, 1);
		cachedForward = forward;
		right = rotationMatrix * Vector3(1, 0, 0);
		cachedRight = right;
	}else if (cachedRotation != rotation) {
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * Vector3(0, 1, 0);
		cachedUp = up;
		forward = rotationMatrix * Vector3(0, 0, 1);
		cachedForward = forward;
		right = rotationMatrix * Vector3(1, 0, 0);
		cachedRight = right;
	} else if (cachedUp != up) {
		rotation = Quaternion::FromTwoVectors(cachedUp, up);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		up = Vector3::Normalize(up);
		cachedUp = up;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		forward = rotationMatrix * Vector3(0, 0, 1);
		cachedForward = forward;
		right = rotationMatrix * Vector3(1, 0, 0);
		cachedRight = right;
	} else if (cachedForward != forward) {
		rotation = Quaternion::FromTwoVectors(cachedForward, forward);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		forward = Vector3::Normalize(forward);
		cachedForward = forward;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * Vector3(0, 1, 0);
		cachedUp = up;
		right = rotationMatrix * Vector3(1, 0, 0);
		cachedRight = right;
	} else if (cachedRight != right) {
		rotation = Quaternion::FromTwoVectors(cachedRight, right);
		cachedRotation = rotation;
		rotationEuler = Quaternion::ToEuler(rotation);
		cachedRotationEuler = rotationEuler;
		right = Vector3::Normalize(right);
		cachedRight = right;
		Matrix3x3 rotationMatrix = (Matrix3x3)Quaternion::ToRotationMatrix(rotation);
		up = rotationMatrix * Vector3(0, 1, 0);
		cachedUp = up;
		forward = rotationMatrix * Vector3(0, 0, 1);
		cachedForward = forward;
	}
}

void KritiaEngine::Transform::OnInspector() {
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::InputFloat3("##Position", &position.x);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", &rotationEuler.x);
		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::InputFloat3("##Scale", &scale.x);
		ImGui::TreePop();
	} 

}

std::string KritiaEngine::Transform::Serialize() {
	json json;
	json["Type"] = "Transform";
	json["Position"] = { position.x, position.y, position.z };
	json["Rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
	json["Scale"] = { scale.x, scale.y, scale.z };
	return json.dump();
}

void KritiaEngine::Transform::Deserialize(const json& json) {
	this->position = Vector3(json["Position"][0], json["Position"][1], json["Position"][2]);
	this->rotation = Quaternion(json["Rotation"][0], json["Rotation"][1], json["Rotation"][2], json["Rotation"][3]);
	this->scale = Vector3(json["Scale"][0], json["Scale"][1], json["Scale"][2]);
}
