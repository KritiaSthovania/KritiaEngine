#include "BoxCollider.h"
#include "../Transform.h"
#include "../../Editor/ImguiAlias.h"
#include "../../CoreModule/Mesh.h"
#include "../MeshFilter.h"
using namespace KritiaEngine;

KritiaEngine::BoxCollider::BoxCollider(GameObject* gameObject) {
    this->gameObject = gameObject;
    center = Vector3(0, 0, 0);
    size = Vector3(1, 1, 1);
    ComponentUpdate();
    Manager::PhysicsManager::AddCollider(this);
    Manager::BehaviourManager::AddUpdatableComponents(this);
}

Vector3 KritiaEngine::BoxCollider::GetExtremePoint(PointPos pos) {
    Matrix3x3 rot = Transform()->rotationMatrix;
    Bound bound = Bound();
    if (gameObject->GetComponent<MeshFilter>() != nullptr && gameObject->GetComponent<MeshFilter>()->mesh != nullptr) {
        bound = gameObject->GetComponent<MeshFilter>()->mesh->bound;
    } 
    switch (pos) {
    case ForwardBottomLeft:
        return Transform()->position + center + rot * Vector3(-1, -1,  1) * bound.GetExtent();
    case ForwardBottomRight:
        return Transform()->position + center + rot * Vector3(1,  -1,  1) * bound.GetExtent();
    case ForwardTopLeft: 
        return Transform()->position + center + rot * Vector3(-1,  1,  1) * bound.GetExtent();
    case ForwardTopRight:
        return Transform()->position + center + rot * Vector3( 1,  1,  1) * bound.GetExtent();
    case BackBottomLeft:
        return Transform()->position + center + rot * Vector3(-1, -1, -1) * bound.GetExtent();
    case BackBottomRight:
        return Transform()->position + center + rot * Vector3( 1, -1, -1) * bound.GetExtent();
    case BackTopLeft:
        return Transform()->position + center + rot * Vector3(-1,  1, -1) * bound.GetExtent();
    case BackTopRight:
        return Transform()->position + center + rot * Vector3( 1,  1, -1) * bound.GetExtent();
    }
}

void KritiaEngine::BoxCollider::ComponentUpdate() {
    UpdateBound();
}

void KritiaEngine::BoxCollider::OnInspector() {
    Behaviour::OnInspector();
    ImguiAlias::FloatField3("Center", &center.x);
    ImguiAlias::FloatField3("Size", &size.x);
    ImguiAlias::BoolField("Is Trigger", &isTrigger);
}

std::string KritiaEngine::BoxCollider::SerializeToJson() {
    json json = json::parse(Behaviour::SerializeToJson());
    json["Type"] = "BoxCollider";
    json["Center"] = { center.x, center.y, center.z };
    json["Size"] = { size.x, size.y, size.z };
    json["IsTrigger"] = isTrigger;
    return json.dump();
}

void KritiaEngine::BoxCollider::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "BoxCollider");
    Behaviour::DeserializeFromJson(json);
    center = Vector3(json["Center"][0], json["Center"][1], json["Center"][2]);
    size = Vector3(json["Size"][0], json["Size"][1], json["Size"][2]);
    isTrigger = json["IsTrigger"];
}

std::string KritiaEngine::BoxCollider::GetInspectorLabel() {
    return "BoxCollider";
}

void KritiaEngine::BoxCollider::UpdateBound() {
    bound.center = Transform()->position + center;
    Vector3 min = Vector3(std::min({ GetExtremePoint(ForwardBottomLeft).x, GetExtremePoint(ForwardBottomRight).x, GetExtremePoint(ForwardTopLeft).x,
        GetExtremePoint(ForwardTopRight).x, GetExtremePoint(BackBottomLeft).x, GetExtremePoint(BackBottomRight).x, GetExtremePoint(BackTopLeft).x, GetExtremePoint(BackTopRight).x }),
        std::min({ GetExtremePoint(ForwardBottomLeft).y, GetExtremePoint(ForwardBottomRight).y, GetExtremePoint(ForwardTopLeft).y,
        GetExtremePoint(ForwardTopRight).y, GetExtremePoint(BackBottomLeft).y, GetExtremePoint(BackBottomRight).y, GetExtremePoint(BackTopLeft).y, GetExtremePoint(BackTopRight).y }),
        std::min({ GetExtremePoint(ForwardBottomLeft).z, GetExtremePoint(ForwardBottomRight).z, GetExtremePoint(ForwardTopLeft).z,
        GetExtremePoint(ForwardTopRight).z, GetExtremePoint(BackBottomLeft).z, GetExtremePoint(BackBottomRight).z, GetExtremePoint(BackTopLeft).z, GetExtremePoint(BackTopRight).z }));
    Vector3 max = Vector3(std::max({ GetExtremePoint(ForwardBottomLeft).x, GetExtremePoint(ForwardBottomRight).x, GetExtremePoint(ForwardTopLeft).x,
        GetExtremePoint(ForwardTopRight).x, GetExtremePoint(BackBottomLeft).x, GetExtremePoint(BackBottomRight).x, GetExtremePoint(BackTopLeft).x, GetExtremePoint(BackTopRight).x }),
        std::max({ GetExtremePoint(ForwardBottomLeft).y, GetExtremePoint(ForwardBottomRight).y, GetExtremePoint(ForwardTopLeft).y,
        GetExtremePoint(ForwardTopRight).y, GetExtremePoint(BackBottomLeft).y, GetExtremePoint(BackBottomRight).y, GetExtremePoint(BackTopLeft).y, GetExtremePoint(BackTopRight).y }),
        std::max({ GetExtremePoint(ForwardBottomLeft).z, GetExtremePoint(ForwardBottomRight).z, GetExtremePoint(ForwardTopLeft).z,
        GetExtremePoint(ForwardTopRight).z, GetExtremePoint(BackBottomLeft).z, GetExtremePoint(BackBottomRight).z, GetExtremePoint(BackTopLeft).z, GetExtremePoint(BackTopRight).z }));
    bound.size = max - min;
}
