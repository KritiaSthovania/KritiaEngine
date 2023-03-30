#include "GameObject.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "../Component/Component.h"
#include "../Component/ComponentFactory.h"
#include "../Editor/EditorApplication.h"
#include "Manager/GameObjectManager.h"
#include "SceneManager.h"
#include "../Editor/ImguiAlias.h"
#include <imgui/imgui.h>
#include <fstream>
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
using namespace nlohmann::literals;
using json = nlohmann::ordered_json;

KritiaEngine::GameObject::GameObject() {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = "New GameObject";
	SceneManagement::SceneManager::GetActiveScene()->GetRootGameObjects().push_back(this);
}

KritiaEngine::GameObject::GameObject(const char* name) {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = name;
	SceneManagement::SceneManager::GetActiveScene()->GetRootGameObjects().push_back(this);
}

std::shared_ptr<KritiaEngine::Transform> KritiaEngine::GameObject::Transform()
{
	return this->GetComponent<KritiaEngine::Transform>();
}

void KritiaEngine::GameObject::GameObject::SetActive(bool isActive)
{
	this->isActive = isActive;
}

std::string KritiaEngine::GameObject::SerializeToJson() {
	json json;
	json["Type"] = "GameObject";
	json["Name"] = this->name;
	if (guid == "") {
		GenerateGuid();
	}
	json["Guid"] = guid;
	json["HasPrefab"] = hasPrefab;
	json["Path"] = path;
	json["Number Of Components"] = this->components.size();
	int componentIndex = 0;
	for (std::shared_ptr<Component> comp : components) {
		json["Component" + std::to_string(componentIndex)] = comp->SerializeToJson();
		componentIndex++;
	}
	return json.dump();
}

void KritiaEngine::GameObject::SerializeToFile() {
	hasPrefab = true;
	if (path == "") {
		std::string prefabPath = ImguiAlias::OpenSaveResourceWindow("Prefabs", KritiaEngine::Editor::prefabFilePostfix, name.c_str());
		if (!prefabPath.ends_with(KritiaEngine::Editor::prefabFilePostfix)) {
			prefabPath += ("/" + (std::string)KritiaEngine::Editor::prefabFilePostfix);
		}
		this->path = prefabPath;
	}
	std::string jsonStr = SerializeToJson();
	std::fstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	output << jsonStr << std::endl;
	output.close();
}

GameObject* KritiaEngine::GameObject::DeserializeFromJson(const json& json) {
	GameObject* gameObject = new GameObject();
	assert(json["Type"] == "GameObject");
	gameObject->name = json["Name"];
	gameObject->guid = json["Guid"];
	gameObject->hasPrefab = json["HasPrefab"];
	gameObject->path = json["Path"];
	int numberOfComponents = json["Number Of Components"];
	int componentIndex = 0;
	for (int i = 0; i < numberOfComponents; i++) {
		nlohmann::ordered_json componentJson = json::parse((std::string)json["Component" + std::to_string(i)]);
		gameObject->AddComponentFromJson(componentJson);
		(*gameObject->components.rbegin())->DeserializeFromJson(componentJson);
	}
	return gameObject;
}

GameObject* KritiaEngine::GameObject::DeserializeFromPath(const std::string& path) {
	std::ifstream input(path);
	json json = json::parse(input);
	input.close();
	return DeserializeFromJson(json);
}

GameObject* KritiaEngine::GameObject::DeserializeAsPrefab(const json& json) {
	GameObject* go = DeserializeFromJson(json);
	SceneManagement::SceneManager::GetActiveScene()->GetRootGameObjects().remove(go);
	return go;
}

void KritiaEngine::GameObject::AddComponentFromJson(const json& json) {
	ComponentFactory::AddComponent(json, this);
}

void KritiaEngine::GameObject::OnObjectDestroy() {
	for (std::shared_ptr<Component> comp : components) {
		Destroy(comp.get());
	}
	SceneManagement::SceneManager::GetActiveScene()->GetRootGameObjects().remove(this);
	delete this;
}

void KritiaEngine::GameObject::OnInspector() {
	ImGui::Checkbox("##IsActive", &isActive);
	ImGui::SameLine();
	ImGui::Text(name.c_str());
	int componentIndex = 0;
	for (std::shared_ptr<Component> comp : components) {
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx(comp->GetInspectorLabel().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup(std::format("DeleteComponent{}", std::to_string(componentIndex)).c_str());
		}
		if (ImGui::BeginPopup(std::format("DeleteComponent{}", std::to_string(componentIndex)).c_str())) {
			if (ImGui::Selectable("Delete")) {
				componentToDestroy.push_back(comp);
			}
			ImGui::EndPopup();
		}
		if (open) {
			comp->OnInspector();
			ImGui::TreePop();
		}
		componentIndex++;
	}
	for (std::shared_ptr<Component> comp : componentToDestroy) {
		Destroy((Object*)comp.get());
	}
	for (std::shared_ptr<Component> comp : componentToDelete) {
		components.remove(comp);
	}
	componentToDelete.clear();
	if (hasPrefab) {
		if (ImGui::Button("Save To Prefab", ImVec2(ImGui::GetWindowContentRegionWidth(), 50))) {
			SerializeToFile();
		}
	}
	ImGui::Separator();
	if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowContentRegionWidth(), 50))) {
		ImGui::OpenPopup("AddComponentMenu");
	}
	if (ImGui::BeginPopup("AddComponentMenu")) {
		ShowAddComponentPopup();
		ImGui::EndPopup();
	}
}

void KritiaEngine::GameObject::ShowAddComponentPopup() {
	for (std::string compType : ComponentFactory::componentType) {
		if (ImGui::Selectable(compType.c_str())) {
			ComponentFactory::AddComponent(compType, this);
		}
	}
}






