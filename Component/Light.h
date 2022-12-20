#pragma once

#include "../Component/Component.h"
#include "../CoreModule/Utilities.h"
#include "../Component/Behaviour.h"
#include "../CoreModule/MathStructs.h"
namespace KritiaEngine {

	enum LightType {
		Directional,
		Spot,
		Point
	};

	class Light : public Behaviour
	{
	public:
		Light(GameObject *gameObject);
		LightType type;
		Color color;
		/// <summary>
		/// Important for directional light and spot light
		/// </summary>
		Vector3 direction = Vector3::Normalize(Vector3(1, 1, 1));
		float ambientIntensity = 0.1f;
		float diffuseIntensity = 0.5f;
		float specularIntensity = 1;
		// for point light
		float constantAttenuationFactor = 1;
		float linearAttenuationFactor = 0.14f;
		float quadraticAttenuationFactor = 0.07f;
		// for spotlight
		float cutOffAngleInner = 15;
		float cutOffAngleOuter = 20;
	};
}


