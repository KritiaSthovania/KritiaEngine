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
		static void AddMeshRenderer(const std::weak_ptr<MeshRenderer>& renderer, bool transparent);
		static void RemoveMeshRenderer(const std::weak_ptr<MeshRenderer>& renderer, bool transparent);
		static void MoveMeshRendererToTransparentQueue(const std::weak_ptr<MeshRenderer>& renderer);
		static void Render();
		/// <summary>
		/// Clear all stored renderes (e.g. on load of new scenes).
		/// </summary>
		static void Clear();
	private:
		static bool ComparePtr(const std::weak_ptr<Renderer>& left, const std::weak_ptr<Renderer>& right);
		static bool CompareRenderer(const std::weak_ptr<Renderer>& left, const std::weak_ptr<Renderer>& right);
		static std::list<std::weak_ptr<Renderer>> opaqueRenderQueue;
		static std::list<std::weak_ptr<Renderer>> transparentRenderQueue;
		static void RenderSkybox();
	};
}



