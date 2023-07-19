#pragma once
#include "Behaviour.h"
namespace KritiaEngine {
	class MonoBehaviour : public Behaviour
	{
		friend class KritiaEngine::Manager::BehaviourManager;
	protected:
		MonoBehaviour(GameObject *gameObject);
		virtual ~MonoBehaviour() = default;
		//����Ⱦ��һ֡ǰ����
		virtual void Awake() = 0;
		virtual void Start() = 0;
		//ÿ֡����
		virtual void Update() = 0;
		virtual void OnDestroy() = 0;
	protected:
		void OnObjectDestroy();
	private:

		void BehaviourAwake();
		void BehaviourStart();
		void BehaviourUpdate();
		bool awoken = false;
		bool started = false;

		// ͨ�� Behaviour �̳�
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "MonoBehaviour";
	};

}

