#include "MeshFilter.h"
#include "../Rendering/RenderManager.h"

KritiaEngine::MeshFilter::MeshFilter(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void KritiaEngine::MeshFilter::SetupMesh() {
	if (mesh != nullptr && !mesh->isSetup) {
		RenderManager::SetupMesh(mesh);
	}
}
