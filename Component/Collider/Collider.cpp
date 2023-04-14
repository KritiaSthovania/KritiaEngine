#include "Collider.h"
#include "../Transform.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

const Bound& KritiaEngine::Collider::GetBound() const {
	return bound;
}

Collision KritiaEngine::Collider::CheckCollision(Collider* other) {
	Collision collision = Collision();
	return boundingVolume->CheckCollision(&collision, other->boundingVolume.get(), Transform(), other->Transform());
}

void KritiaEngine::Collider::OnObjectDestroy() {
	Component::OnObjectDestroy();
	PhysicsManager::RemoveCollider(this);
}
