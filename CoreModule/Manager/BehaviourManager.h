#pragma once
#include <list>
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
		// update updatable components
		static void ComponentUpdate();
		/// <summary>
		/// Clear all MonoBehaviors and Components in the queues (e.g. on load of new scenes).
		/// </summary>
		static void Clear();
	private:
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviours;
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviourStartList;
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviourAwakeList;
		static std::list<KritiaEngine::Component*> updatableComponents;
		static std::list<KritiaEngine::MonoBehaviour*> monoBehaviourToDelete;
		static std::list<KritiaEngine::Component*> updatableComponentsToDelete;

	};
}



