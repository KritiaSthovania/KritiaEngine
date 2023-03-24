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

bool KritiaEngine::MeshFilter::IsMeshSetup() {
	return mesh->isSetup;
}

void KritiaEngine::MeshFilter::OnInspector() {
	ImGui::Text("Mesh ");
	ImGui::SameLine();
	if (ImGui::Button(mesh->name == "" ? "mesh" : mesh->name.c_str())) {
		std::string path = ImguiAlias::OpenFindResourceWindow("Mesh", KritiaEngine::Editor::meshFilePostfix);
		if (path != "") {
			mesh = std::shared_ptr<Mesh>(new Mesh());
			mesh->DeserializeFromPath(path);
		}
	}
	//if (ImGui::BeginDragDropTarget()) {

	//	ImGui::EndDragDropTarget();
	//}
}

std::string KritiaEngine::MeshFilter::SerializeToJson() {
	json json;
	json["Type"] = "MeshFilter";
	json["Mesh"] = mesh->SerializeToJson();
	return json.dump();
}

void KritiaEngine::MeshFilter::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "MeshFilter");
	mesh = std::shared_ptr<Mesh>(new Mesh());
	nlohmann::ordered_json meshJson = json::parse((std::string)json["Mesh"]);
	mesh->DeserializeFromJson(meshJson);
}

std::string KritiaEngine::MeshFilter::GetInspectorLabel() {
	return inspectorLabel;
}
