#include "Texture.h"
#include "../Editor/EditorApplication.h"
#include "../Editor/ImguiAlias.h"
#include <fstream>
using namespace KritiaEngine;
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;
KritiaEngine::Texture::Texture(const std::string& path)
{
	this->path = path;
}

void KritiaEngine::Texture::LoadImage(const std::string& path)
{
	this->path = path;
}

std::string KritiaEngine::Texture::SerializeToJson() {
	json json;
	json["Type"] = "Texture";
	json["Path"] = path;
	return json.dump();
}
void KritiaEngine::Texture::DeserializeFromPath(const std::string& path) {
	std::ifstream input(path);
	json json = json::parse(input);
	DeserializeFromJson(json);
	input.close();
}

void KritiaEngine::Texture::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "Texture");
	path = json["Path"];
}

void KritiaEngine::Texture::SerializeToFile() {
	std::string jsonStr = SerializeToJson();
	std::string path = ImguiAlias::OpenSaveResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix, "New Texture");
	if (!path.ends_with(KritiaEngine::Editor::textureFilePostfix)) {
		path += ("/" + (std::string)KritiaEngine::Editor::textureFilePostfix);
	}
	std::fstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	output << jsonStr << std::endl;
	output.close();
}

