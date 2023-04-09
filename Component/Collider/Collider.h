#pragma once
#include "../Behaviour.h"
#include "../../CoreModule/Utilities.h"
#include "../../CoreModule/Manager/PhysicsManager.h"
namespace KritiaEngine {
	class Collider : public Behaviour {
	public:
		Bound bound;
		//virtual void OnCollisionEnter() = 0;
		//virtual void OnCollisionStay() = 0;
		//virtual void OnCollisionExit() = 0;
	private:
		virtual void OnObjectDestroy() override;
	};
}


