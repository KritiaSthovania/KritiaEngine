#pragma once
#include "../Mesh.h"
#include "../Material.h"
#include <json/json.hpp>
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
		static std::shared_ptr<Material> GetMaterialFromPath(std::string path);
		static std::shared_ptr<Material> GetMaterialFromJson(const nlohmann::ordered_json& json);
	private:
		static std::shared_ptr<Mesh> cube;
		static std::list<std::shared_ptr<Material>> materials;
	};
}

