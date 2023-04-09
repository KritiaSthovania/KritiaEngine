#pragma once
#include "Component.h"
namespace KritiaEngine {
	class Behaviour : public Component
	{
	public:
		bool enabled = true;
	protected:
		virtual void OnInspector() override;

		// ͨ�� Component �̳�
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
	};

}


