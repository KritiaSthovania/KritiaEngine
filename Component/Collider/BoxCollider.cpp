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
        return Transform()->position + center + rot * (Vector3(-1, -1,  1) * Transform()->scale) * bound.GetExtent() * size ;
    case ForwardBottomRight:
        return Transform()->position + center + rot * (Vector3(1,  -1,  1) * Transform()->scale) * bound.GetExtent() * size;
    case ForwardTopLeft: 
        return Transform()->position + center + rot * (Vector3(-1,  1,  1) * Transform()->scale) * bound.GetExtent() * size ;
    case ForwardTopRight:
        return Transform()->position + center + rot * (Vector3( 1,  1,  1) * Transform()->scale) * bound.GetExtent() * size;
    case BackBottomLeft:
        return Transform()->position + center + rot * (Vector3(-1, -1, -1) * Transform()->scale) * bound.GetExtent() * size;
    case BackBottomRight:
        return Transform()->position + center + rot * (Vector3( 1, -1, -1) * Transform()->scale) * bound.GetExtent() * size;
    case BackTopLeft:
        return Transform()->position + center + rot * (Vector3(-1,  1, -1) * Transform()->scale) * bound.GetExtent() * size;
    case BackTopRight:
        return Transform()->position + center + rot * (Vector3( 1,  1, -1) * Transform()->scale) * bound.GetExtent() * size;
    }
}

void KritiaEngine::BoxCollider::ComponentUpdate() {
    if (cachedPosition != Transform()->position || cachedRotation != Transform()->rotation || cachedScale != Transform()->scale || cachedBoundingVolumeDepth != boundingVolumeDepth) {
        boundingVolume = nullptr;
        UpdateBound();
        UpdateBoundingVolume();
        cachedPosition = Transform()->position;
        cachedRotation = Transform()->rotation;
        cachedScale = Transform()->scale;
        cachedBoundingVolumeDepth = boundingVolumeDepth;
    }
}

void KritiaEngine::BoxCollider::OnInspector() {
    Collider::OnInspector();
    ImguiAlias::FloatField3("Center", &center.x);
    ImguiAlias::FloatField3("Size", &size.x);
    ImguiAlias::IntField("Bounding Volume Depth", &boundingVolumeDepth);
    ImguiAlias::BoolField("Is Trigger", &isTrigger);
}

std::string KritiaEngine::BoxCollider::SerializeToJson() {
    json json = json::parse(Collider::SerializeToJson());
    json["Type"] = "BoxCollider";
    json["Center"] = { center.x, center.y, center.z };
    json["Size"] = { size.x, size.y, size.z };
    json["BoundingVolumeDepth"] = boundingVolumeDepth;
    json["IsTrigger"] = isTrigger;
    return json.dump();
}

void KritiaEngine::BoxCollider::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "BoxCollider");
    Collider::DeserializeFromJson(json);
    center = Vector3(json["Center"][0], json["Center"][1], json["Center"][2]);
    size = Vector3(json["Size"][0], json["Size"][1], json["Size"][2]);
    boundingVolumeDepth = json["BoundingVolumeDepth"];
    isTrigger = json["IsTrigger"];
    UpdateBoundingVolume();
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
    if (boundingVolume == nullptr) {
        // Order:  ForwardBottomLeft, ForwardBottomRight, ForwardTopLeft, ForwardTopRight, BackBottomLeft, BackBottomRight, BackTopLeft, BackTopRight
        boundingVolume = std::make_unique<BoundingVolumeOBB>(BoundingVolumeOBB());
        boundingVolume->vertices.push_back(GetVertex(ForwardBottomLeft));
        boundingVolume->vertices.push_back(GetVertex(ForwardBottomRight));
        boundingVolume->vertices.push_back(GetVertex(ForwardTopLeft));
        boundingVolume->vertices.push_back(GetVertex(ForwardTopRight));
        boundingVolume->vertices.push_back(GetVertex(BackBottomLeft));
        boundingVolume->vertices.push_back(GetVertex(BackBottomRight));
        boundingVolume->vertices.push_back(GetVertex(BackTopLeft));
        boundingVolume->vertices.push_back(GetVertex(BackTopRight));
        boundingVolume->CreateChildren(boundingVolumeDepth, 0);
    } else {
        boundingVolume->vertices[0] = GetVertex(ForwardBottomLeft);
        boundingVolume->vertices[1] = GetVertex(ForwardBottomRight);
        boundingVolume->vertices[2] = GetVertex(ForwardTopLeft);
        boundingVolume->vertices[3] = GetVertex(ForwardTopRight);
        boundingVolume->vertices[4] = GetVertex(BackBottomLeft);
        boundingVolume->vertices[5] = GetVertex(BackBottomRight);
        boundingVolume->vertices[6] = GetVertex(BackTopLeft);
        boundingVolume->vertices[7] = GetVertex(BackTopRight);
        boundingVolume->UpdateChildren(boundingVolumeDepth, 0);
    }
    
}


