#include "RigidBody.h"
#include "Collider/BoxCollider.h"
#include "../CoreModule/Manager/PhysicsManager.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
RigidBody::RigidBody(GameObject* gameObject) {
	this->gameObject = gameObject;
	PhysicsManager::AddRigidBody(this);
	ComputeInertiaTensor();
}

void KritiaEngine::RigidBody::AddForce(const Vector3& force) {
	this->force += force;
}

void KritiaEngine::RigidBody::AddTorque(const Vector3& position, const Vector3& force) {
	if (std::dynamic_pointer_cast<BoxCollider>(collider) != nullptr) {
		this->torque += Vector3::Cross(position - (Transform()->position + std::dynamic_pointer_cast<BoxCollider>(collider)->center), force);

	}
}

void KritiaEngine::RigidBody::AddForceAndTorque(const Vector3& position, const Vector3& force) {
	AddForce(force);
	AddTorque(position, force);
}

void KritiaEngine::RigidBody::ComputeInertiaTensor() {
	if (mass > 0) {
		if (gameObject->GetComponent<BoxCollider>() != nullptr) {
			collider = gameObject->GetComponent<BoxCollider>();
			boxColliderSize = dynamic_cast<BoxCollider*>(collider.get())->size;
			if (gameObject->GetComponent<MeshFilter>() != nullptr) {
				meshFilter = gameObject->GetComponent<MeshFilter>();
				if (gameObject->GetComponent<MeshFilter>()->mesh != nullptr) {
					mesh = gameObject->GetComponent<MeshFilter>()->mesh;
					float compX = dynamic_cast<BoxCollider*>(collider.get())->size.x * mesh->bound.size.x * Transform()->scale.x;
					float compY = dynamic_cast<BoxCollider*>(collider.get())->size.y * mesh->bound.size.y * Transform()->scale.y;
					float compZ = dynamic_cast<BoxCollider*>(collider.get())->size.z * mesh->bound.size.z * Transform()->scale.z;
					inertiaTensor = mass * Vector3(compY * compY + compZ * compZ, compX * compX + compZ * compZ, compX * compX + compY * compY) / 12.f;
				}
			} else {
				inertiaTensor = Vector3::Zero();
			}
		} else {
			inertiaTensor = Vector3::Zero();
		}
	} else {
		inertiaTensor = Vector3(1, 1, 1);
	}

}

void KritiaEngine::RigidBody::PhysicsUpdate() {
	if (meshFilter != gameObject->GetComponent<MeshFilter>() || (gameObject->GetComponent<MeshFilter>()!= nullptr && mesh != gameObject->GetComponent<MeshFilter>()->mesh) || cachedMass != mass || ColliderChanged()) {
		ComputeInertiaTensor();
		cachedMass = mass;
	}
	if (!isKinematic) {
		// semi-implicit euler
		accelaration = 1.f / mass * force;
		if (useGravity) {
			accelaration += PhysicsManager::gravityAccelaration;
		}
		lastVelocity = velocity;
		velocity += accelaration * PhysicsManager::stepSize;
		Transform()->position += velocity * PhysicsManager::stepSize;
		angularAccelaration = GetInertiaTensorInverseWorld() * torque;
		lastAngularVelocity = angularVelocity;
		angularVelocity += angularAccelaration * PhysicsManager::stepSize;
		Quaternion omegaTilde = Quaternion(angularVelocity.x, angularVelocity.y, angularVelocity.z, 0);
		Transform()->rotation += 0.5 * omegaTilde * Transform()->rotation * PhysicsManager::stepSize;
		Transform()->rotation = Quaternion::Normalize(Transform()->rotation);
		ClearForceAndTorque();
	}
}

void KritiaEngine::RigidBody::ClearForceAndTorque() {
	force = Vector3::Zero();
	torque = Vector3::Zero();
}

bool KritiaEngine::RigidBody::ColliderChanged() {
	if (collider != nullptr) {
		if (dynamic_cast<BoxCollider*>(collider.get()) != nullptr && boxColliderSize != dynamic_cast<BoxCollider*>(collider.get())->size) {
			boxColliderSize = dynamic_cast<BoxCollider*>(collider.get())->size;
			return true;
		}
	}
	return false;
}

Matrix3x3 KritiaEngine::RigidBody::GetInertiaTensorInverseWorld() {
	float entries[3][3] = { 1.f / inertiaTensor.x, 0, 0, 0, 1.f / inertiaTensor.y, 0, 0, 0, 1.f / inertiaTensor.z };
	Matrix3x3 inertiaLocalInverse = Matrix3x3(entries);
	return Transform()->rotationMatrix * inertiaLocalInverse * Transform()->rotationMatrix.Transpose();
}

void KritiaEngine::RigidBody::OnInspector() {
	ImguiAlias::FloatField("Mass", &mass);
	ImguiAlias::BoolField("Use Gravity", &useGravity);
	ImguiAlias::BoolField("Is Kinematic", &isKinematic);
}

std::string KritiaEngine::RigidBody::SerializeToJson() {
	json json;
	json["Type"] = "RigidBody";
	json["Mass"] = mass;
	json["IsKinematic"] = isKinematic;
	json["UseGravity"] = useGravity;
	return json.dump();
}

void KritiaEngine::RigidBody::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "RigidBody");
	mass = json["Mass"];
	isKinematic = json["IsKinematic"];
	useGravity = json["UseGravity"];
	ComputeInertiaTensor();
}

std::string KritiaEngine::RigidBody::GetInspectorLabel() {
	return "RigidBody";
}

void KritiaEngine::RigidBody::OnObjectDestroy() {
	Component::OnObjectDestroy();
	PhysicsManager::RemoveRigidBody(this);
}
