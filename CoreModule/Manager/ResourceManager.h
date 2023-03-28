#pragma once
#include "../Mesh.h"
#include "../Material.h"
#include "../GameObject.h"
#include <json/json.hpp>
#include <pch.h>
#include <list>

namespace KritiaEngine::Editor {
	class AssetDatabase;
}

namespace KritiaEngine::Manager {

	class ResourceManager {
		friend class KritiaEngine::Editor::AssetDatabase;
	public:
		enum class MeshPrimitive {
			Cube
		};
		// Mesh primitives
		static std::shared_ptr<Mesh> GetMeshPrimitive(MeshPrimitive type);
		static std::shared_ptr<Mesh> GetMesh(const std::string& path);
		static std::shared_ptr<Mesh> GetMesh(const nlohmann::ordered_json& json);
		static std::shared_ptr<Material> GetMaterial(const std::string& path);
		static std::shared_ptr<Material> GetMaterial(const nlohmann::ordered_json& json);
		static std::shared_ptr<Texture> GetTexture(const std::string& path);
		static std::shared_ptr<Texture> GetTexture(const nlohmann::ordered_json& json);
		static GameObject* GetPrefab(const std::string& path);
	private:
		static std::shared_ptr<Mesh> cube;
		static std::list<std::shared_ptr<Mesh>> meshes;
		static std::list<std::shared_ptr<Material>> materials;
		static std::list<std::shared_ptr<Texture>> textures;
		static std::list<GameObject*> prefabs;
	};
}

