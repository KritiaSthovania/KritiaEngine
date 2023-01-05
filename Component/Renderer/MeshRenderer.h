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
		std::vector<std::shared_ptr<Material>> materials;
	protected:
		void Render(const std::shared_ptr<KritiaEngine::Camera>& camera);
		void RenderShadowMap(const std::shared_ptr<Light>& light);
		void OnObjectDestroy();
	private:
		void Initialize();
		std::shared_ptr<MeshFilter> meshFilter;
		// Preinitailize materials if there is already a mesh on the game object
		// This makes the order of materials correct and can initialize the containTransparentMaterial field.
		void PreInitializeMaterial();
		void InitializeMaterial();
		// Update materials if there are new ones.
		void UpdateMaterial();
		bool initialized = false;
		bool preInitialized = false;
		bool containTransparentMaterial = false;
		// indicate whether there are new materials
		int materialSize = 0;
	};
}


