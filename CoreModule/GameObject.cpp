#include "GameObject.h"
#include "../Component/Renderer/MeshRenderer.h"

using namespace KritiaEngine;

KritiaEngine::GameObject::GameObject() {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = "New Game Object";
}

KritiaEngine::GameObject::GameObject(const char* name) {
	components.push_back(std::shared_ptr<KritiaEngine::Transform>(new KritiaEngine::Transform(this)));
	this->name = name;
}

std::shared_ptr<KritiaEngine::Transform> KritiaEngine::GameObject::Transform()
{
	return this->GetComponent<KritiaEngine::Transform>();
}

void KritiaEngine::GameObject::GameObject::SetActive(bool isActive)
{
	IsActive = isActive;
}
