#include "Collider.h"
#include "../Transform.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

const Bound& KritiaEngine::Collider::GetBound() const {
	return bound;
}

Collision KritiaEngine::Collider::CheckCollision(Collider* other) {
	Collision collision = Collision();
	boundingVolume->CheckCollision(&collision, other->boundingVolume.get(), Transform(), other->Transform());
	return collision;
}

void KritiaEngine::Collider::OnObjectDestroy() {
	Component::OnObjectDestroy();
	delete collisionShape;
	PhysicsManager::RemoveCollider(this);
}

void KritiaEngine::Collider::OnInspector() {
	Behaviour::OnInspector();
	ImguiAlias::FloatField("Friction", &friction);
	ImguiAlias::FloatField("Bounciness", &bounciness);
}

std::string KritiaEngine::Collider::SerializeToJson() {
	json json = json::parse(Behaviour::SerializeToJson());
	json["Friction"] = friction;
	json["Bounciness"] = bounciness;
	return json.dump();
}

void KritiaEngine::Collider::DeserializeFromJson(const json& json) {
	Behaviour::DeserializeFromJson(json);
	friction = json["Friction"];
	bounciness = json["Bounciness"];
}