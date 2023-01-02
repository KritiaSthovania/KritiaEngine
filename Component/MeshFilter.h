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
	};
}


