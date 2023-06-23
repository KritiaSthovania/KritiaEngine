#include "RigidBody.h"
#include "Collider/BoxCollider.h"
#include "../CoreModule/Manager/PhysicsManager.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
RigidBody::RigidBody(GameObject* gameObject) {
	this->gameObject = gameObject;
	ComputeInertiaTensor();
	btTransform transform;
	transform.setOrigin(btVector3(Transform()->position.x, Transform()->position.y, Transform()->position.z));
	transform.setRotation(btQuaternion(Transform()->rotation.x, Transform()->rotation.y, Transform()->rotation.z, Transform()->rotation.w));
	ms = new btDefaultMotionState(transform);
	if (dynamic_cast<BoxCollider*>(gameObject->GetComponent<BoxCollider>().get()) != nullptr) {
		collider = gameObject->GetComponent<BoxCollider>();
	}
	PhysicsManager::AddRigidBody(this);
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

void KritiaEngine::RigidBody::SetKinematic(bool kinematic) {
	if (kinematic) {
		isKinematic = true;
		mass = 1.0;
	} else {
		mass = 0.0;
		isKinematic = false;
	}
}

void KritiaEngine::RigidBody::ComputeInertiaTensor() {
	if (mass > 0) {
		if (gameObject->GetComponent<BoxCollider>() != nullptr) {
			collider = gameObject->GetComponent<BoxCollider>();
			boxColliderSize = dynamic_cast<BoxCollider*>(collider.get())->size;
			btVector3 localInertia(0, 0, 0);
			collider->collisionShape->calculateLocalInertia(mass, localInertia);
			inertiaTensor = Vector3(localInertia.x(), localInertia.y(), localInertia.z());
		} else {
			inertiaTensor = Vector3::Zero();
		}
	} else {
		inertiaTensor = Vector3(1, 1, 1);
	}

}

void KritiaEngine::RigidBody::PhysicsUpdate() {
	if (!initialized) {
		initialized = true;
		btRigidBody::btRigidBodyConstructionInfo info(btScalar(mass), ms, collider->collisionShape, btVector3(inertiaTensor.x, inertiaTensor.y, inertiaTensor.z));
		btRB = new btRigidBody(info);
		PhysicsManager::AddRigidBodyBullet(this);
	}
	if (collider->isTrigger) {
		//
	} else {
		if (!isKinematic) {
			if (!useGravity) {
				btRB->setGravity(btVector3(0, 0, 0));
			} else {
				btRB->setGravity(btVector3(PhysicsManager::gravityAccelaration.x, PhysicsManager::gravityAccelaration.y, PhysicsManager::gravityAccelaration.z));
			}
			Transform()->position = Vector3(btRB->getWorldTransform().getOrigin().x(), btRB->getWorldTransform().getOrigin().y(), btRB->getWorldTransform().getOrigin().z());
			Transform()->rotation = Quaternion(btRB->getWorldTransform().getRotation().x(), btRB->getWorldTransform().getRotation().y(), btRB->getWorldTransform().getRotation().z(), btRB->getWorldTransform().getRotation().w());
		}
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
	delete btRB;
	delete ms;
	Component::OnObjectDestroy();
	PhysicsManager::RemoveRigidBody(this);
}
