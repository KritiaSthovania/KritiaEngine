#include "GameObject.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "../Component/Component.h"
#include "../Component/ComponentFactory.h"
#include "../Editor/EditorApplication.h"
#include "Manager/GameObjectManager.h"
#include "SceneManager.h"
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
	IsActive = isActive;
}

std::string KritiaEngine::GameObject::SerializeToJson() {
	json json;
	json["Type"] = "GameObject";
	json["Name"] = this->name;
	json["Number Of Components"] = this->components.size();
	int componentIndex = 0;
	for (std::shared_ptr<Component> comp : components) {
		json["Component" + std::to_string(componentIndex)] = comp->SerializeToJson();
		componentIndex++;
	}
	return json.dump();
}

void KritiaEngine::GameObject::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "GameObject");
	this->name = json["Name"];
	int numberOfComponents = json["Number Of Components"];
	int componentIndex = 0;
	for (int i = 0; i < numberOfComponents; i++) {
		nlohmann::ordered_json componentJson = json::parse((std::string)json["Component" + std::to_string(i)]);
		this->AddComponentFromJson(componentJson);
		(*this->components.rbegin())->DeserializeFromJson(componentJson);
	}
}

void KritiaEngine::GameObject::AddComponentFromJson(const json& json) {
	ComponentFactory::AddComponentFromJson(json, this);
}

void KritiaEngine::GameObject::SerializeToFile() {
	std::string jsonStr = SerializeToJson();
	std::string path = ImguiAlias::OpenFindResourceWindow("Prefab", KritiaEngine::Editor::prefabFilePostfix);
	if (!path.ends_with(KritiaEngine::Editor::prefabFilePostfix)) {
		path += ("/" + (std::string)KritiaEngine::Editor::prefabFilePostfix);
	}
	std::fstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	output << jsonStr << std::endl;
	output.close();
}

void KritiaEngine::GameObject::OnObjectDestroy() {
	for (std::shared_ptr<Component> comp : components) {
		Destroy(comp.get());
	}
	SceneManagement::SceneManager::GetActiveScene()->GetRootGameObjects().remove(this);
}





