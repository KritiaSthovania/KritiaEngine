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

Vector3 KritiaEngine::BoxCollider::GetPoint(PointPos pos) {
    Vector3 forwardTopRight = Vector3(Transform()->right.x + Transform()->up.x + Transform()->forward.x, Transform()->right.y + Transform()->up.y + Transform()->forward.y, Transform()->right.z + Transform()->up.z + Transform()->forward.z);
    forwardTopRight = Vector3(forwardTopRight.x * size.x, forwardTopRight.y * size.y, forwardTopRight.z * size.z);
    Bound bound = Bound();
    if (gameObject->GetComponent<MeshFilter>() != nullptr && gameObject->GetComponent<MeshFilter>()->mesh != nullptr) {
        bound = gameObject->GetComponent<MeshFilter>()->mesh->bound;
    } 
    switch (pos) {
    case ForwardBottomLeft:
        return Vector3(Transform()->position.x + center.x - forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y - forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z + forwardTopRight.z * bound.GetExtent().z);
    case ForwardBottomRight:
        return Vector3(Transform()->position.x + center.x + forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y - forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z + forwardTopRight.z * bound.GetExtent().z);
    case ForwardTopLeft: 
        return Vector3(Transform()->position.x + center.x - forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y + forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z + forwardTopRight.z * bound.GetExtent().z);
    case ForwardTopRight:
        return Vector3(Transform()->position.x + center.x + forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y + forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z + forwardTopRight.z * bound.GetExtent().z);
    case BackBottomLeft:
        return Vector3(Transform()->position.x + center.x - forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y - forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z - forwardTopRight.z * bound.GetExtent().z);
    case BackBottomRight:
        return Vector3(Transform()->position.x + center.x + forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y - forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z - forwardTopRight.z * bound.GetExtent().z);
    case BackTopLeft:
        return Vector3(Transform()->position.x + center.x - forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y + forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z - forwardTopRight.z * bound.GetExtent().z);
    case BackTopRight:
        return Vector3(Transform()->position.x + center.x + forwardTopRight.x * bound.GetExtent().x, Transform()->position.y + center.y + forwardTopRight.y * bound.GetExtent().y, Transform()->position.z + center.z - forwardTopRight.z * bound.GetExtent().z);
    }
}

void KritiaEngine::BoxCollider::ComponentUpdate() {
    bound.center = Transform()->position + center;
    Vector3 min = Vector3(std::min({ GetPoint(ForwardBottomLeft).x, GetPoint(ForwardBottomRight).x, GetPoint(ForwardTopLeft).x,
        GetPoint(ForwardTopRight).x, GetPoint(BackBottomLeft).x, GetPoint(BackBottomRight).x, GetPoint(BackTopLeft).x, GetPoint(BackTopRight).x }),
        std::min({ GetPoint(ForwardBottomLeft).y, GetPoint(ForwardBottomRight).y, GetPoint(ForwardTopLeft).y,
        GetPoint(ForwardTopRight).y, GetPoint(BackBottomLeft).y, GetPoint(BackBottomRight).y, GetPoint(BackTopLeft).y, GetPoint(BackTopRight).y }),
        std::min({ GetPoint(ForwardBottomLeft).z, GetPoint(ForwardBottomRight).z, GetPoint(ForwardTopLeft).z,
        GetPoint(ForwardTopRight).z, GetPoint(BackBottomLeft).z, GetPoint(BackBottomRight).z, GetPoint(BackTopLeft).z, GetPoint(BackTopRight).z }));
    Vector3 max = Vector3(std::max({ GetPoint(ForwardBottomLeft).x, GetPoint(ForwardBottomRight).x, GetPoint(ForwardTopLeft).x,
        GetPoint(ForwardTopRight).x, GetPoint(BackBottomLeft).x, GetPoint(BackBottomRight).x, GetPoint(BackTopLeft).x, GetPoint(BackTopRight).x }),
        std::max({ GetPoint(ForwardBottomLeft).y, GetPoint(ForwardBottomRight).y, GetPoint(ForwardTopLeft).y,
        GetPoint(ForwardTopRight).y, GetPoint(BackBottomLeft).y, GetPoint(BackBottomRight).y, GetPoint(BackTopLeft).y, GetPoint(BackTopRight).y }),
        std::max({ GetPoint(ForwardBottomLeft).z, GetPoint(ForwardBottomRight).z, GetPoint(ForwardTopLeft).z,
        GetPoint(ForwardTopRight).z, GetPoint(BackBottomLeft).z, GetPoint(BackBottomRight).z, GetPoint(BackTopLeft).z, GetPoint(BackTopRight).z }));
    bound.size = max - min;
    Debug::Log(GetPoint(BackBottomLeft));
    Debug::Log(bound.GetMin());
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
