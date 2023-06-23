#pragma once

#include "Component.h"
#include "../CoreModule/Mesh.h"

namespace KritiaEngine::Manager {
	class PhysicsManager;
}


namespace KritiaEngine {
	class DeformableBody : public Component {
		friend class Manager::PhysicsManager;
	public:
		enum class SolverType {
			ShapeMatching = 0
		};
		DeformableBody(GameObject* gameObject);
		SolverType solver = SolverType::ShapeMatching;
		float stiffness = 0.02;
		

	private:
		std::shared_ptr<Mesh> mesh;

		void PhysicsUpdate();

		virtual void OnObjectDestroy() override;
		// Í¨¹ý Component ¼Ì³Ð
		virtual void OnInspector() override;

		virtual std::string SerializeToJson() override;

		virtual void DeserializeFromJson(const json& json) override;

		virtual std::string GetInspectorLabel() override;

	};
}
