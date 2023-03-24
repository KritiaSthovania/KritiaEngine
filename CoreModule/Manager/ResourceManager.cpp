#include "ResourceManager.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

std::shared_ptr<Mesh> Manager::ResourceManager::cube = nullptr;
std::list<std::shared_ptr<Material>> ResourceManager::materials;

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

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterialFromPath(std::string path) {
	for (std::shared_ptr<Material> mat : materials) {
		if (mat->path == path) {
			return mat;
		}
	}
	std::shared_ptr<Material> mat = Material::DeserializeFromPath(path);
	materials.push_back(mat);
	return mat;
}

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterialFromJson(const nlohmann::ordered_json& json) {
	return Material::DeserializeFromJson(json);
}
