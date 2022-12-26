#pragma once
#include <pch.h>
namespace KritiaEngine {
	class MonoBehaviour;
	class Behaviour;
	class Component;
}

namespace KritiaEngine::Manager{
	class BehaviourManager {
	public:
		static void AddMonoBehaviour(KritiaEngine::MonoBehaviour* behaviour);
		static void RemoveMonoBehaviour(KritiaEngine::MonoBehaviour* behaviour);
		static void AddUpdatableComponents(KritiaEngine::Component* component);
		static void RemoveUpdatableComponents(KritiaEngine::Component* component);
		static void BehaviourUpdate();
	private:
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviours;
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviourStartQueue;
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviourAwakeQueue;
		static std::list<KritiaEngine::Component*> updatableComponents;

	};
}



