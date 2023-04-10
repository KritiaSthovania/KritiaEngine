#include "BoxCollider.h"
#include "../Transform.h"
#include "../../Editor/ImguiAlias.h"
using namespace KritiaEngine;

KritiaEngine::BoxCollider::BoxCollider(GameObject* gameObject) {
    this->gameObject = gameObject;
    center = Vector3(0, 0, 0);
    size = Vector3(1, 1, 1);
    ComponentUpdate();
    Manager::PhysicsManager::AddCollider(this);
    Manager::BehaviourManager::AddUpdatableComponents(this);
}

void KritiaEngine::BoxCollider::ComponentUpdate() {
    bound.center = Transform()->position + center;
    bound.size = Vector3(Transform()->scale.x * size.x, Transform()->scale.y * size.y, Transform()->scale.z * size.z);
}

void KritiaEngine::BoxCollider::OnInspector() {
    Behaviour::OnInspector();
    ImguiAlias::FloatField3("Center", &center.x);
    ImguiAlias::FloatField3("Size", &size.x);
}

std::string KritiaEngine::BoxCollider::SerializeToJson() {
    json json = json::parse(Behaviour::SerializeToJson());
    json["Type"] = "BoxCollider";
    json["Center"] = { center.x, center.y, center.z };
    json["Size"] = { size.x, size.y, size.z };
    return json.dump();
}

void KritiaEngine::BoxCollider::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "BoxCollider");
    Behaviour::DeserializeFromJson(json);
    center = Vector3(json["Center"][0], json["Center"][1], json["Center"][2]);
    size = Vector3(json["Size"][0], json["Size"][1], json["Size"][2]);
}

std::string KritiaEngine::BoxCollider::GetInspectorLabel() {
    return "BoxCollider";
}
