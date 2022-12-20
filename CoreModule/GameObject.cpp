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

void KritiaEngine::GameObject::Render(std::shared_ptr<Camera> camera) {
	if (IsActive) {
		for (std::list<std::shared_ptr<Component>>::iterator iter = components.begin(); iter != components.end(); iter++) {
			if (typeid(*(*iter).get()) == typeid(MeshRenderer)) {
				std::shared_ptr<MeshRenderer> renderer = std::dynamic_pointer_cast<MeshRenderer>(*iter);
				renderer->Render(camera);
			}
		}
	}
}

void KritiaEngine::GameObject::GameObject::SetActive(bool isActive)
{
	IsActive = isActive;
}
