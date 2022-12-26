#pragma once
#include "Behaviour.h"
namespace KritiaEngine {
	class MonoBehaviour : public Behaviour
	{
		friend class KritiaEngine::Manager::BehaviourManager;
	protected:
		MonoBehaviour(GameObject *gameObject);
		//在渲染第一帧前调用
		virtual void Awake() = 0;
		virtual void Start() = 0;
		//每帧调用
		virtual void Update() = 0;
		virtual void OnDestroy() = 0;
	private:
		void OnObjectDestroy();
		void BehaviourAwake();
		void BehaviourStart();
		void BehaviourUpdate();
		bool awoken = false;
		bool started = false;
	};

}

