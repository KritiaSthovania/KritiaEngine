#pragma once
#include "Component.h"
#include "../CoreModule/Utilities.h"
#include "MeshFilter.h"

namespace KritiaEngine::Manager {
	class PhysicsManager;
}

namespace KritiaEngine {
	class RigidBody : public Component {
		friend class Manager::PhysicsManager;
	public:
		RigidBody(GameObject* gameObject);

		float mass = 1;
		Vector3 inertiaTensor = Vector3::Zero();
		bool isKinematic = false;
		bool useGravity = true;
		void AddForce(const Vector3& force);
		void AddTorque(const Vector3& position, const Vector3& force);
		void AddForceAndTorque(const Vector3& position, const Vector3& force);
		const Vector3& GetVelocity() const {
			return velocity;
		}
		const Vector3& GetAngularVelocity() const {
			return angularVelocity;
		}
	private:
		void ComputeInertiaTensor();
		void PhysicsUpdate();
		void ClearForceAndTorque();
		bool ColliderChanged();
		Matrix3x3 GetInertiaTensorInverseWorld();
		float cachedMass = 0;
		Vector3 force = Vector3::Zero();
		Vector3 torque = Vector3::Zero();
		Vector3 velocity = Vector3::Zero();
		Vector3 lastVelocity = Vector3::Zero();
		Vector3 angularVelocity = Vector3::Zero();
		Vector3 lastAngularVelocity = Vector3::Zero();
		Vector3 accelaration = Vector3::Zero();
		Vector3 angularAccelaration = Vector3::Zero();
		std::shared_ptr<MeshFilter> meshFilter = nullptr;
		std::shared_ptr<Mesh> mesh = nullptr;
		std::shared_ptr<Collider> collider = nullptr;
		// to check if collider has changed
		Vector3 boxColliderSize = Vector3(1, 1, 1);

		// Í¨¹ý Component ¼Ì³Ð
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		virtual void OnObjectDestroy() override;
	};
}


