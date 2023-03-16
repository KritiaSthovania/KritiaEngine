#pragma once

#include "pch.h"
#include "../Component/Light.h"
namespace KritiaEngine::Lighting {

	class LightingSystem {
	public:
		static const int MaxSpotLightsForOneObject;
		static const int MaxPointLightsForOneObject;

		static std::list<Light*> Lights;
		static void AddLight(Light* light);
		static void RemoveLight(Light* light);
		static std::vector<Light*> GetSpotLightAroundPos(const Vector3& pos);
		static std::vector<Light*> GetPointLightAroundPos(const Vector3& pos);
		static Light* GetMainLightSource();
		static void SetMainLightSource(Light* light);
		static void Reset();
	private:
		/// <summary>
		/// The main light source of the current scene.Intended for rendering purposes.
		/// </summary>		
		static Light* MainLightSource;
	};
}

