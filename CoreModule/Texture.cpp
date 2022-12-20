#include "Texture.h"

KritiaEngine::Texture::Texture(std::string path)
{
	this->path = path;
}

void KritiaEngine::Texture::LoadImage(std::string path)
{
	this->path = path;
}

