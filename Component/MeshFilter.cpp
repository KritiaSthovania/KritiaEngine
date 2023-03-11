#include "MeshFilter.h"
#include "../Rendering/RenderingProvider.h"
using namespace KritiaEngine::Rendering;

KritiaEngine::MeshFilter::MeshFilter(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void KritiaEngine::MeshFilter::SetupMesh() {
	if (mesh != nullptr && !mesh->isSetup) {
		RenderingProvider::SetupMesh(mesh);
	}
}

void KritiaEngine::MeshFilter::OnInspector() {}

std::string KritiaEngine::MeshFilter::Serialize() {
	return std::string();
}

void KritiaEngine::MeshFilter::Deserialize(const json& json) {}

std::string KritiaEngine::MeshFilter::GetInspectorLabel() {
	return inspectorLabel;
}
