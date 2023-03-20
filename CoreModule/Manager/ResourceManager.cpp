#include "ResourceManager.h"

using namespace KritiaEngine;

std::shared_ptr<Mesh> Manager::ResourceManager::cube = nullptr;

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
