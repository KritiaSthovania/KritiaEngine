#pragma once
#include <pch.h>
#include "../../Component/Camera.h"
namespace KritiaEngine {
	class Renderer;
	class MeshRenderer;
}

namespace KritiaEngine::Manager {
	class RendererManager {
	public:
		static void AddMeshRenderer(MeshRenderer* renderer, bool transparent);
		static void RemoveMeshRenderer(MeshRenderer* renderer, bool transparent);
		static void MoveMeshRendererToTransparentQueue(MeshRenderer* renderer);
		static void Render();
	private:
		static bool CompareRenderer(Renderer * left, Renderer * right);
		static std::list<Renderer*> opaqueRenderQueue;
		static std::list<Renderer*> transparentRenderQueue;
		static void RenderSkybox();
	};
}



