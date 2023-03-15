#include "Texture.h"
#include <fstream>
using namespace KritiaEngine;

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

