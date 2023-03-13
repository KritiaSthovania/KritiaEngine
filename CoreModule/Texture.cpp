#include "Texture.h"
using namespace KritiaEngine;

KritiaEngine::Texture::Texture(const std::string& path)
{
	this->path = path;
}

void KritiaEngine::Texture::LoadImage(const std::string& path)
{
	this->path = path;
}

std::string KritiaEngine::Texture::Serialize() {
	return std::string();
}

std::shared_ptr<Texture> KritiaEngine::Texture::DeserializeFromPath(const std::string& path) {
	return std::shared_ptr<Texture>();
}

