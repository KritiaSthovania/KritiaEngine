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
    UpdateBound();
    UpdateBoundingVolume();
    Manager::PhysicsManager::AddCollider(this);
    Manager::BehaviourManager::AddUpdatableComponents(this);
}


Vector3 KritiaEngine::BoxCollider::GetVertex(PointPos pos) {
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
    if (cachedPosition != Transform()->position || cachedRotation != Transform()->rotation || cachedScale != Transform()->scale || cachedBoundingVolumeDepth != boundingVolumeDepth) {
        UpdateBound();
        UpdateBoundingVolume();
        cachedPosition = Transform()->position;
        cachedRotation = Transform()->rotation;
        cachedScale = Transform()->scale;
        cachedBoundingVolumeDepth = boundingVolumeDepth;
    }
}

void KritiaEngine::BoxCollider::OnInspector() {
    Behaviour::OnInspector();
    ImguiAlias::FloatField3("Center", &center.x);
    ImguiAlias::FloatField3("Size", &size.x);
    ImguiAlias::IntField("Bounding Volume Depth", &boundingVolumeDepth);
    ImguiAlias::BoolField("Is Trigger", &isTrigger);
}

std::string KritiaEngine::BoxCollider::SerializeToJson() {
    json json = json::parse(Behaviour::SerializeToJson());
    json["Type"] = "BoxCollider";
    json["Center"] = { center.x, center.y, center.z };
    json["Size"] = { size.x, size.y, size.z };
    json["BoundingVolumeDepth"] = boundingVolumeDepth;
    json["IsTrigger"] = isTrigger;
    return json.dump();
}

void KritiaEngine::BoxCollider::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "BoxCollider");
    Behaviour::DeserializeFromJson(json);
    center = Vector3(json["Center"][0], json["Center"][1], json["Center"][2]);
    size = Vector3(json["Size"][0], json["Size"][1], json["Size"][2]);
    boundingVolumeDepth = json["BoundingVolumeDepth"];
    isTrigger = json["IsTrigger"];
}

std::string KritiaEngine::BoxCollider::GetInspectorLabel() {
    return "BoxCollider";
}

void KritiaEngine::BoxCollider::UpdateBound() {
    bound.center = Transform()->position + center;
    Vector3 min = Vector3(std::min({ GetVertex(ForwardBottomLeft).x, GetVertex(ForwardBottomRight).x, GetVertex(ForwardTopLeft).x,
        GetVertex(ForwardTopRight).x, GetVertex(BackBottomLeft).x, GetVertex(BackBottomRight).x, GetVertex(BackTopLeft).x, GetVertex(BackTopRight).x }),
        std::min({ GetVertex(ForwardBottomLeft).y, GetVertex(ForwardBottomRight).y, GetVertex(ForwardTopLeft).y,
        GetVertex(ForwardTopRight).y, GetVertex(BackBottomLeft).y, GetVertex(BackBottomRight).y, GetVertex(BackTopLeft).y, GetVertex(BackTopRight).y }),
        std::min({ GetVertex(ForwardBottomLeft).z, GetVertex(ForwardBottomRight).z, GetVertex(ForwardTopLeft).z,
        GetVertex(ForwardTopRight).z, GetVertex(BackBottomLeft).z, GetVertex(BackBottomRight).z, GetVertex(BackTopLeft).z, GetVertex(BackTopRight).z }));
    Vector3 max = Vector3(std::max({ GetVertex(ForwardBottomLeft).x, GetVertex(ForwardBottomRight).x, GetVertex(ForwardTopLeft).x,
        GetVertex(ForwardTopRight).x, GetVertex(BackBottomLeft).x, GetVertex(BackBottomRight).x, GetVertex(BackTopLeft).x, GetVertex(BackTopRight).x }),
        std::max({ GetVertex(ForwardBottomLeft).y, GetVertex(ForwardBottomRight).y, GetVertex(ForwardTopLeft).y,
        GetVertex(ForwardTopRight).y, GetVertex(BackBottomLeft).y, GetVertex(BackBottomRight).y, GetVertex(BackTopLeft).y, GetVertex(BackTopRight).y }),
        std::max({ GetVertex(ForwardBottomLeft).z, GetVertex(ForwardBottomRight).z, GetVertex(ForwardTopLeft).z,
        GetVertex(ForwardTopRight).z, GetVertex(BackBottomLeft).z, GetVertex(BackBottomRight).z, GetVertex(BackTopLeft).z, GetVertex(BackTopRight).z }));
    bound.size = max - min;
}

void KritiaEngine::BoxCollider::UpdateBoundingVolume() {
    // split direction: current depth is even: top-bottom, current depth is odd: left-right
    // we have here depth 0, so child1 is top, child2 is bottom
    if (boundingVolume1 == nullptr) {
        // Order:  ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight
        // top
        boundingVolume1 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
        boundingVolume1->vertices.push_back(0.5 * Vector3(GetVertex(ForwardBottomLeft) + GetVertex(ForwardTopLeft)));
        boundingVolume1->vertices.push_back(0.5 * Vector3(GetVertex(ForwardBottomRight) + GetVertex(ForwardTopRight)));
        boundingVolume1->vertices.push_back(Vector3(GetVertex(ForwardTopLeft)));
        boundingVolume1->vertices.push_back(Vector3(GetVertex(ForwardTopRight)));
        boundingVolume1->vertices.push_back(0.5 * Vector3(GetVertex(BackBottomLeft) + GetVertex(BackTopLeft)));
        boundingVolume1->vertices.push_back(0.5 * Vector3(GetVertex(BackBottomRight) + GetVertex(BackTopRight)));
        boundingVolume1->vertices.push_back(Vector3(GetVertex(BackTopLeft)));
        boundingVolume1->vertices.push_back(Vector3(GetVertex(BackTopRight)));
        boundingVolume1->CreateChildren(boundingVolumeDepth, 1);
    }
    if (boundingVolume2 == nullptr) {
        // bottom
        boundingVolume2 = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
        boundingVolume2->vertices.push_back(Vector3(GetVertex(ForwardBottomLeft)));
        boundingVolume2->vertices.push_back(Vector3(GetVertex(ForwardBottomRight)));
        boundingVolume2->vertices.push_back(0.5 * Vector3(GetVertex(ForwardTopLeft)) + Vector3(GetVertex(ForwardBottomLeft)));
        boundingVolume2->vertices.push_back(0.5 * Vector3(GetVertex(ForwardTopRight)) + Vector3(GetVertex(ForwardBottomRight)));
        boundingVolume2->vertices.push_back(Vector3(GetVertex(BackBottomLeft)));
        boundingVolume2->vertices.push_back(Vector3(GetVertex(BackBottomRight)));
        boundingVolume2->vertices.push_back(0.5 * Vector3(GetVertex(BackTopLeft)) + Vector3(GetVertex(BackBottomLeft)));
        boundingVolume2->vertices.push_back(0.5 * Vector3(GetVertex(BackTopRight)) + Vector3(GetVertex(BackBottomRight)));
        boundingVolume2->CreateChildren(boundingVolumeDepth, 1);
    } else {
        boundingVolume1->vertices[0] = 0.5 * Vector3(GetVertex(ForwardBottomLeft) + GetVertex(ForwardTopLeft));
        boundingVolume1->vertices[1] = 0.5 * Vector3(GetVertex(ForwardBottomRight) + GetVertex(ForwardTopRight));
        boundingVolume1->vertices[2] = Vector3(GetVertex(ForwardTopLeft));
        boundingVolume1->vertices[3] = Vector3(GetVertex(ForwardTopRight));
        boundingVolume1->vertices[4] = 0.5 * Vector3(GetVertex(BackBottomLeft) + GetVertex(BackTopLeft));
        boundingVolume1->vertices[5] = 0.5 * Vector3(GetVertex(BackBottomRight) + GetVertex(BackTopRight));
        boundingVolume1->vertices[6] = Vector3(GetVertex(BackTopLeft));
        boundingVolume1->vertices[7] = Vector3(GetVertex(BackTopRight));
        boundingVolume1->UpdateChildren(boundingVolumeDepth, 1);

        boundingVolume2->vertices[0] = Vector3(GetVertex(ForwardBottomLeft));
        boundingVolume2->vertices[1] = Vector3(GetVertex(ForwardBottomRight));
        boundingVolume2->vertices[2] = 0.5 * Vector3(GetVertex(ForwardTopLeft)) + Vector3(GetVertex(ForwardBottomLeft));
        boundingVolume2->vertices[3] = 0.5 * Vector3(GetVertex(ForwardTopRight)) + Vector3(GetVertex(ForwardBottomRight));
        boundingVolume2->vertices[4] = Vector3(GetVertex(BackBottomLeft));
        boundingVolume2->vertices[5] = Vector3(GetVertex(BackBottomRight));
        boundingVolume2->vertices[6] = 0.5 * Vector3(GetVertex(BackTopLeft)) + Vector3(GetVertex(BackBottomLeft));
        boundingVolume2->vertices[7] = 0.5 * Vector3(GetVertex(BackTopRight)) + Vector3(GetVertex(BackBottomRight));
        boundingVolume2->UpdateChildren(boundingVolumeDepth, 1);
    }
    
}
