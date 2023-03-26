#include "ResourceManager.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

std::shared_ptr<Mesh> Manager::ResourceManager::cube = nullptr;
std::list<std::shared_ptr<Mesh>> ResourceManager::meshes;
std::list<std::shared_ptr<Material>> ResourceManager::materials;
std::list<std::shared_ptr<Texture>> ResourceManager::textures;

std::shared_ptr<Mesh> KritiaEngine::Manager::ResourceManager::GetMeshPrimitive(MeshPrimitive type) {
	switch (type) {
	case MeshPrimitive::Cube:
		if (cube == nullptr) {
			cube = std::make_shared<Mesh>(Mesh::Cube());
		}
		return cube;
	}
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Mesh> KritiaEngine::Manager::ResourceManager::GetMesh(const std::string& path) {
	for (std::shared_ptr<Mesh> mesh : meshes) {
		if (mesh->path == path) {

			return mesh;
		}
	}
	std::shared_ptr<Mesh> mesh = Mesh::DeserializeFromPath(path);
	meshes.push_back(mesh);
	return mesh;
}

std::shared_ptr<Mesh> KritiaEngine::Manager::ResourceManager::GetMesh(const nlohmann::ordered_json& json) {
	if (json["IsPrimitive"] == false) {
		for (std::shared_ptr<Mesh> mesh : meshes) {
			if (mesh->path == json["Path"]) {
				return mesh;
			}
		}
		std::shared_ptr<Mesh> mesh = Mesh::DeserializeFromJson(json);
		meshes.push_back(mesh);
	} else {
		if (json["Path"] == "Cube") {
			return GetMeshPrimitive(MeshPrimitive::Cube);
		}
	}
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterial(const std::string& path) {
	for (std::shared_ptr<Material> mat : materials) {
		if (mat->path == path) {
			return mat;
		}
	}
	std::shared_ptr<Material> mat = Material::DeserializeFromPath(path);
	materials.push_back(mat);
	return mat;
}

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterial(const nlohmann::ordered_json& json) {
	return Material::DeserializeFromJson(json);
}

std::shared_ptr<Texture> KritiaEngine::Manager::ResourceManager::GetTexture(const std::string& path) {
	for (std::shared_ptr<Texture> tex : textures) {
		if (tex->path == path) {
			return tex;
		}
	}
	std::shared_ptr<Texture> tex = Texture::DeserializeFromPath(path);
	textures.push_back(tex);
	return tex;
}

std::shared_ptr<Texture> KritiaEngine::Manager::ResourceManager::GetTexture(const nlohmann::ordered_json& json) {
	return Texture::DeserializeFromJson(json);
}