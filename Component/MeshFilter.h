#pragma once
#include "Component.h"
#include "../CoreModule/Mesh.h"
#include "../CoreModule/Material.h"

namespace KritiaEngine {
	class MeshFilter : public Component {
		friend class MeshRenderer;
	public:
		MeshFilter(GameObject* gameObject);
		std::shared_ptr<Mesh> mesh;
	private:
		void RenderSubmesh(int index);
		// Should be called by MeshRenderer
		void SetupMesh();
	};
}


