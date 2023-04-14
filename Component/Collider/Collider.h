#pragma once
#include "BoundingVolume.h"
#include "../Behaviour.h"
#include "../../CoreModule/Utilities.h"
#include "../../CoreModule/Manager/PhysicsManager.h"

namespace KritiaEngine {
	class Collider : public Behaviour {
		friend class KritiaEngine::Manager::PhysicsManager;
	public:
		const Bound& GetBound() const;
		bool isTrigger = false;
	protected:
		Bound bound;
		std::unique_ptr<BoundingVolume> boundingVolume = nullptr;
	private:
		Collision CheckCollision(Collider* other);
		virtual void OnObjectDestroy() override;
		virtual void UpdateBound() = 0;
		virtual void UpdateBoundingVolume() = 0;
	};
}


