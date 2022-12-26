#include "Texture.h"

KritiaEngine::Texture::Texture(const std::string& path)
{
	this->path = path;
}

void KritiaEngine::Texture::LoadImage(const std::string& path)
{
	this->path = path;
}

