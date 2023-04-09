#include "Collider.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

void KritiaEngine::Collider::OnObjectDestroy() {
	PhysicsManager::RemoveCollider(this);
}
