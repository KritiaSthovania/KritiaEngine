#pragma once

#include "pch.h"
#include "../Component/Light.h"
namespace KritiaEngine::Lighting {

	static class LightingSystem {
	public:
		static const int MaxSpotLightsForOneObject;
		static const int MaxPointLightsForOneObject;
		/// <summary>
		/// The main light source of the current scene.Intended for rendering purposes.
		/// </summary>		
		static std::shared_ptr<Light> MainLightSource;
		static std::list<std::shared_ptr<Light>> SpotLights;
		static std::list<std::shared_ptr<Light>> PointLights;

		static std::vector<std::shared_ptr<Light>> GetSpotLightAroundPos(const Vector3& pos);
		static std::vector<std::shared_ptr<Light>> GetPointLightAroundPos(const Vector3& pos);
	};
}

