#pragma once

#include "../Component/Component.h"
#include "../CoreModule/Utilities.h"
#include "../Component/Behaviour.h"
#include "../CoreModule/MathStructs.h"

namespace KritiaEngine::Rendering {
	class OpenGLRendering;
}

namespace KritiaEngine {

	enum class LightType {
		Directional,
		Spot,
		Point
	};

	class Light : public Behaviour
	{
		friend class KritiaEngine::Rendering::OpenGLRendering;
	public:
		Light(GameObject *gameObject);
		/// <summary>
		/// Get a light space matrix. For point light, also need a direction.
		/// </summary>
		/// <param name="direction">0: right; 1: left; 2: top; 3: bottom; 4: near; 5: far</param>
		Matrix4x4 GetLightMatrixVP(int direction);
		LightType type;
		Color color;
		float ambientIntensity = 0.1f;
		float diffuseIntensity = 0.5f;
		float specularIntensity = 1;
		bool castingShadow = true;
		// for point light and spot light
		float constantAttenuationFactor = 1;
		float linearAttenuationFactor = 0.14f;
		float quadraticAttenuationFactor = 0.07f;
		// for spotlight
		float cutOffAngleInner = 15;
		float cutOffAngleOuter = 20;

	protected:
		void OnObjectDestroy();
	private:
		unsigned int shadowMapID = 0;
		unsigned int shadowMapFBO = 0;
		unsigned int shadowMapPointID = 0;
		unsigned int shadowMapPointFBO = 0;
		Matrix4x4 directionalLightMatrix, spotLightMatrix, pointLightMatrixRight, pointLightMatrixLeft, pointLightMatrixTop, pointLightMatrixBottom, pointLightMatrixNear, pointLightMatrixFar;
		Vector3 cachedPosition;
		Quaternion cachedRotation;
		void UpdateLightMatrices();

		// Í¨¹ý Behaviour ¼Ì³Ð
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "Light";
	};
}


