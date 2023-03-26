#include "Texture.h"
#include "../Editor/EditorApplication.h"
#include "../Editor/ImguiAlias.h"
#include "../Rendering/RenderingProvider.h"
#include <fstream>
#include <imgui/imgui.h>
using namespace KritiaEngine;
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;
KritiaEngine::Texture::Texture(const std::string& path)
{
	name = "New Texture";
	this->path = path;
}

void KritiaEngine::Texture::LoadImage(const std::string& path, bool alphaChannel)
{
	this->path = path;
	ID = Rendering::RenderingProvider::Load2DTextureFromPath(path, alphaChannel);
	loaded = true;
}

std::string KritiaEngine::Texture::SerializeToJson() {
	json json;
	json["Type"] = "Texture";
	json["Name"] = name;
	json["Path"] = path;
	return json.dump();
}
std::shared_ptr<Texture> KritiaEngine::Texture::DeserializeFromPath(const std::string& path) {
	std::ifstream input(path);
	json json = json::parse(input);
	input.close();
	return DeserializeFromJson(json);
}

std::shared_ptr<Texture> KritiaEngine::Texture::DeserializeFromJson(const json& json) {
	std::shared_ptr<Texture> tex = std::make_shared<Texture>(Texture());
	assert(json["Type"] == "Texture");
	tex->name = json["Name"];
	tex->path = json["Path"];
	return tex;
}

void KritiaEngine::Texture::SerializeToFile() {
	std::string jsonStr = SerializeToJson();
	std::string path = ImguiAlias::OpenSaveResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix, name == "" ? "New Texture" : name.c_str());
	if (!path.ends_with(KritiaEngine::Editor::textureFilePostfix)) {
		path += ("/" + (std::string)KritiaEngine::Editor::textureFilePostfix);
	}
	std::fstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	output << jsonStr << std::endl;
	output.close();
}

void KritiaEngine::Texture::OnInspector() {
	ImGui::Text(((std::string)("Texture:   " + name)).c_str());
	ImGui::Separator();
	ImGui::Text("Path: ");
	ImGui::SameLine();
	ImGui::Text(path.c_str());
	if (!loaded) {
		LoadImage(path, false);
	}
	ImGui::Image((void*)ID, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionWidth()));
}

