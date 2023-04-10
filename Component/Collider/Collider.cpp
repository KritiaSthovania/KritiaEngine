#include "Collider.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

const Bound& KritiaEngine::Collider::GetBound() const {
	return bound;
}

void KritiaEngine::Collider::OnObjectDestroy() {
	Component::OnObjectDestroy();
	PhysicsManager::RemoveCollider(this);
}
