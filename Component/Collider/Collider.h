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
		float friction = 0.3;
		float bounciness = 0.8;
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
	private:
		Collision CheckCollision(Collider* other);
		virtual void OnObjectDestroy() override;
		virtual void UpdateBound() = 0;
		virtual void UpdateBoundingVolume() = 0;

	};
}


