#pragma once

#include "../../Rendering/Shader.h"
#include "../../CoreModule/Lighting.h"
#include "../../CoreModule/Settings.h"
#include "../Transform.h"
#include "Renderer.h"
#include "../MeshFilter.h"
#include "../../CoreModule/Material.h"
namespace KritiaEngine {
	class MeshRenderer : public Renderer {
	public:
		MeshRenderer(GameObject* gameObject);
		~MeshRenderer() {
		}
		void Render(std::shared_ptr<KritiaEngine::Camera> camera);
		void Initialize();
		std::vector<std::shared_ptr<Material>> materials;
	private:
		std::shared_ptr<MeshFilter> meshFilter;
		void InitializeMaterial();
		bool initialized = false;
	};
}


