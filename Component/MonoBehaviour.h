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

		// 通过 Behaviour 继承
		virtual void OnInspector() override;
		virtual std::string Serialize() override;
		virtual void Deserialize(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "MonoBehaviour";
	};

}

