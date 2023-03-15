#pragma once
#include "Component.h"
#include "../CoreModule/Mesh.h"

namespace KritiaEngine {
	class MeshFilter : public Component {
		friend class MeshRenderer;
	public:
		MeshFilter(GameObject* gameObject);
		std::shared_ptr<Mesh> mesh;
	private:
		// Should be called by MeshRenderer
		void SetupMesh();

		// Í¨¹ý Component ¼Ì³Ð
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "MeshFilter";
	};
}


