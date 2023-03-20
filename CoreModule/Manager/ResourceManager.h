#pragma once
#include "../Mesh.h"
#include <pch.h>
#include <list>
namespace KritiaEngine::Manager {

	class ResourceManager {
	public:
		enum class MeshPrimitive {
			Cube
		};
		// Mesh primitives
		static std::shared_ptr<Mesh> GetMeshPrimitive(MeshPrimitive type);
	private:
		static std::shared_ptr<Mesh> cube;
	};
}

