#include "MeshFilter.h"
#include "../Rendering/RenderingProvider.h"
#include "../Editor/EditorApplication.h"
using namespace KritiaEngine::Rendering;
using namespace KritiaEngine::Editor::GUI;

KritiaEngine::MeshFilter::MeshFilter(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void KritiaEngine::MeshFilter::SetupMesh() {
	if (mesh != nullptr && !mesh->isSetup) {
		RenderingProvider::SetupMesh(mesh);
	}
}

void KritiaEngine::MeshFilter::OnInspector() {
	ImGui::Text("Mesh");
	ImGui::SameLine();
	if (ImGui::BeginDragDropTarget()) {
		if (ImGui::Selectable(mesh->name.c_str())) {
			const char* path = ImguiAlias::OpenFindResourceWindow("Mesh", KritiaEngine::Editor::meshFilePostfix);
			mesh = Mesh::DeserializeFromPath(path);
		}
		ImGui::EndDragDropTarget();
	}
}

std::string KritiaEngine::MeshFilter::Serialize() {
	json json;
	json["Type"] = "MeshFilter";
	json["MeshPath"] = mesh->path;
	return json.dump();
}

void KritiaEngine::MeshFilter::Deserialize(const json& json) {
	assert(json["Type"] == "MeshFilter");
	mesh = std::shared_ptr<Mesh>(new Mesh(json["MeshPath"]));
}

std::string KritiaEngine::MeshFilter::GetInspectorLabel() {
	return inspectorLabel;
}
