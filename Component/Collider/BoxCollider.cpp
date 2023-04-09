#include "BoxCollider.h"
#include "../Transform.h"
#include "../../Editor/ImguiAlias.h"
using namespace KritiaEngine;

KritiaEngine::BoxCollider::BoxCollider(GameObject* gameObject) {
    this->gameObject = gameObject;
    Center() = Transform()->position;
    Size() = Vector3(1, 1, 1);
}

Vector3& KritiaEngine::BoxCollider::Center() {
    return bound.center;
}

Vector3& KritiaEngine::BoxCollider::Size() {
    return bound.size;
}

void KritiaEngine::BoxCollider::OnInspector() {
    Behaviour::OnInspector();
    ImguiAlias::FloatField3("Center", &Center().x);
    ImguiAlias::FloatField3("Size", &Size().x);
}

std::string KritiaEngine::BoxCollider::SerializeToJson() {
    json json = json::parse(Behaviour::SerializeToJson());
    json["Type"] = "BoxCollider";
    json["Center"] = { Center().x, Center().y, Center().z };
    json["Size"] = { Size().x, Size().y, Size().z };
    return json.dump();
}

void KritiaEngine::BoxCollider::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "BoxCollider");
    Behaviour::DeserializeFromJson(json);
    Center() = Vector3(json["Center"][0], json["Center"][1], json["Center"][2]);
    Size() = Vector3(json["Size"][0], json["Size"][1], json["Size"][2]);
}

std::string KritiaEngine::BoxCollider::GetInspectorLabel() {
    return "BoxCollider";
}
