#include "Lighting.h"
#include "../Component/Transform.h"
#include "../CoreModule/GameObject.h"

using namespace KritiaEngine::Lighting;
using namespace KritiaEngine;
std::shared_ptr<KritiaEngine::Light> LightingSystem::MainLightSource = nullptr;
const int LightingSystem::MaxSpotLightsForOneObject = 1;
const int LightingSystem::MaxPointLightsForOneObject = 4;
std::list<std::shared_ptr<Light>> LightingSystem::SpotLights = std::list<std::shared_ptr<Light>>();
std::list<std::shared_ptr<Light>> LightingSystem::PointLights = std::list<std::shared_ptr<Light>>();

std::vector<std::shared_ptr<Light>> LightingSystem::GetSpotLightAroundPos(const Vector3& pos) {
    std::vector<std::shared_ptr<Light>> result = std::vector<std::shared_ptr<Light>>(MaxSpotLightsForOneObject);
    for (int i = 0; i < MaxSpotLightsForOneObject; i++) {
        float distance = FLT_MAX;
        std::shared_ptr<Light> closest;
        for (std::list<std::shared_ptr<Light>>::iterator iter = SpotLights.begin(); iter != SpotLights.end(); iter++) {
            float dist = Vector3::Magnitude((*iter)->Transform()->Position - pos);
            if (dist < distance) {
                closest = *iter;
            }
        }
        result.push_back(closest);
    }
    return result;
}

std::vector<std::shared_ptr<Light>> KritiaEngine::Lighting::LightingSystem::GetPointLightAroundPos(const Vector3& pos) {
    std::vector<std::shared_ptr<Light>> result = std::vector<std::shared_ptr<Light>>(MaxPointLightsForOneObject);
    for (int i = 0; i < MaxPointLightsForOneObject; i++) {
        float distance = FLT_MAX;
        std::shared_ptr<Light> closest;
        for (std::list<std::shared_ptr<Light>>::iterator iter = PointLights.begin(); iter != PointLights.end(); iter++) {
            float dist = Vector3::Magnitude((*iter)->Transform()->Position - pos);
            if (dist < distance) {
                closest = *iter;
            }
        }
        result.push_back(closest);
    }
    return result;
}
