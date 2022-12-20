#include "MeshFilter.h"

KritiaEngine::MeshFilter::MeshFilter(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void KritiaEngine::MeshFilter::RenderSubmesh(int index) {
	mesh->RenderSubmesh(index);
}

void KritiaEngine::MeshFilter::SetupMesh() {
	if (mesh != nullptr && !mesh->isSetup) {
		mesh->Setup();
	}
}
