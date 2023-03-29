#include "ResourceManager.h"
#include "RendererManager.h"
#include "BehaviourManager.h"
#include "../../Component/Renderer/MeshRenderer.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

std::shared_ptr<Mesh> Manager::ResourceManager::cube = nullptr;
std::map<std::string, std::shared_ptr<Mesh>> ResourceManager::meshes;
std::map<std::string, std::shared_ptr<Material>> ResourceManager::materials;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;
std::map<std::string, GameObject*> ResourceManager::prefabs;

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
	return GetMesh(GetJsonFromPath(path));
}

std::shared_ptr<Mesh> KritiaEngine::Manager::ResourceManager::GetMesh(const nlohmann::ordered_json& json) {
	if (json["IsPrimitive"] == false) {
		auto res = meshes.find(json["Guid"]);
		if (res != meshes.end()) {
			return res->second;
		} else {
			std::shared_ptr<Mesh> mesh = Mesh::DeserializeFromJson(json);
			meshes[json["Guid"]] = mesh;
			return mesh;
		}
	} else {
		if (json["Path"] == "Cube") {
			return GetMeshPrimitive(MeshPrimitive::Cube);
		}
	}
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterial(const std::string& path) {
	return GetMaterial(GetJsonFromPath(path));
}

std::shared_ptr<Material> KritiaEngine::Manager::ResourceManager::GetMaterial(const nlohmann::ordered_json& json) {
	auto res = materials.find(json["Guid"]);
	if (res != materials.end()) {
		return res->second;
	} else {
		std::shared_ptr<Material> material = Material::DeserializeFromJson(json);
		materials[json["Guid"]] = material;
		return material;
	}
}

std::shared_ptr<Texture> KritiaEngine::Manager::ResourceManager::GetTexture(const std::string& path) {
	return GetTexture(GetJsonFromPath(path));
}

std::shared_ptr<Texture> KritiaEngine::Manager::ResourceManager::GetTexture(const nlohmann::ordered_json& json) {
	auto res = textures.find(json["Guid"]);
	if (res != textures.end()) {
		return res->second;
	} else {
		std::shared_ptr<Texture> texture = Texture::DeserializeFromJson(json);
		textures[json["Guid"]] = texture;
		return texture;
	}
}

GameObject* KritiaEngine::Manager::ResourceManager::GetPrefab(const std::string& path) {
	return GetPrefab(GetJsonFromPath(path));
}

GameObject* KritiaEngine::Manager::ResourceManager::GetPrefab(const json& json) {
	auto res = prefabs.find(json["Guid"]);
	if (res != prefabs.end()) {
		return res->second;
	} else {
		GameObject* gameObject = GameObject::DeserializeAsPrefab(json);
		prefabs[json["Guid"]] = gameObject;
		RendererManager::RemoveMeshRenderer(gameObject->GetComponent<MeshRenderer>().get(), false);
		RendererManager::RemoveMeshRenderer(gameObject->GetComponent<MeshRenderer>().get(), true);
		for (std::shared_ptr<Component> comp : gameObject->components) {
			if (typeid(comp.get()) == typeid(MonoBehaviour)) {
				BehaviourManager::RemoveMonoBehaviour(dynamic_cast<MonoBehaviour*>(comp.get()));
			}
		}
		return gameObject;
	}
}

nlohmann::ordered_json KritiaEngine::Manager::ResourceManager::GetJsonFromPath(const std::string& path) {
	std::ifstream instream(path);
	json json = json::parse(instream);
	instream.close();
	return json;
}
