#pragma once
#include "Component.h"
#include "Behaviour.h"
namespace KritiaEngine {
	class MonoBehaviour : public Behaviour
	{
	public:
		//����Ⱦ��һ֡ǰ����
		virtual void Start();
		//ÿ֡����
		virtual void Update();
	protected:
	};

}

