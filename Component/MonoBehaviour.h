#pragma once
#include "Component.h"
#include "Behaviour.h"
namespace KritiaEngine {
	class MonoBehaviour : public Behaviour
	{
	public:
		//在渲染第一帧前调用
		virtual void Start();
		//每帧调用
		virtual void Update();
	protected:
	};

}

