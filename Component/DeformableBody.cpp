#include "DeformableBody.h"
#include "../CoreModule/Manager/PhysicsManager.h"
#include "MeshFilter.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

KritiaEngine::DeformableBody::DeformableBody(GameObject* gameObject) {
    this->gameObject = gameObject;
    mesh = gameObject->GetComponent<MeshFilter>()->mesh;
    PhysicsManager::AddDeformableBody(this);
}

void KritiaEngine::DeformableBody::PhysicsUpdate() {}

void KritiaEngine::DeformableBody::OnInspector() {}

std::string KritiaEngine::DeformableBody::SerializeToJson() {
    return std::string();
}

void KritiaEngine::DeformableBody::DeserializeFromJson(const json& json) {}

std::string KritiaEngine::DeformableBody::GetInspectorLabel() {
    return "Deformable Body";
}

void KritiaEngine::DeformableBody::OnObjectDestroy() {
    Component::OnObjectDestroy();
    PhysicsManager::RemoveDeformableBody(this);
}