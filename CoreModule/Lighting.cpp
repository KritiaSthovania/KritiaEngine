#include "Lighting.h"
#include "../Component/Transform.h"
#include "../CoreModule/GameObject.h"

using namespace KritiaEngine::Lighting;
using namespace KritiaEngine;
std::shared_ptr<KritiaEngine::Light> LightingSystem::MainLightSource = nullptr;
const int LightingSystem::MaxSpotLightsForOneObject = 1;
const int LightingSystem::MaxPointLightsForOneObject = 4;
std::list<Light*> LightingSystem::Lights = std::list<Light*>();

void KritiaEngine::Lighting::LightingSystem::AddLight(Light* light) {
    Lights.push_back(light);
}

void KritiaEngine::Lighting::LightingSystem::RemoveLight(Light* light) {
    Lights.remove(light);
}

std::vector<Light*> LightingSystem::GetSpotLightAroundPos(const Vector3& pos) {
    std::vector<Light*> result = std::vector<Light*>();
    std::list<Light*> tmp(Lights);
    for (int i = 0; i < MaxSpotLightsForOneObject; i++) {
        float distance = FLT_MAX;
        Light* closest = nullptr;
        for (std::list<Light*>::iterator iter = Lights.begin(); iter != Lights.end(); iter++) {
            if ((*iter)->type == LightType::Spot) {
                float dist = Vector3::Magnitude((*iter)->Transform()->position - pos);
                if (dist < distance) {
                    closest = *iter;
                }
            }
        }
        if (closest != nullptr) {
            result.push_back(closest);
            tmp.remove(closest);
        }
    }
    return result;
}

std::vector<Light*> KritiaEngine::Lighting::LightingSystem::GetPointLightAroundPos(const Vector3& pos) {
    std::vector<Light*> result = std::vector<Light*>();
    std::list<Light*> tmp(Lights);
    for (int i = 0; i < MaxPointLightsForOneObject; i++) {
        float distance = FLT_MAX;
        Light* closest = nullptr;
        for (std::list<Light*>::iterator iter = tmp.begin(); iter != tmp.end(); iter++) {
            if ((*iter)->type == LightType::Point) {
                float dist = Vector3::Magnitude((*iter)->Transform()->position - pos);
                if (dist < distance) {
                    closest = *iter;
                }
            }
        }
        if (closest != nullptr) {
            result.push_back(closest);
            tmp.remove(closest);
        }
    }
    return result;
}

std::shared_ptr<Light> KritiaEngine::Lighting::LightingSystem::GetMainLightSource() {
    if (MainLightSource != nullptr) {
        return MainLightSource;
    } else {
        for (Light* light : Lights) {
            if (light->type == LightType::Directional) {
                MainLightSource = std::make_shared<Light>(*light);
            }
        }
    }
    return MainLightSource;
}

void KritiaEngine::Lighting::LightingSystem::SetMainLightSource(std::shared_ptr<Light> light) {
    if (light->type == LightType::Directional) {
        MainLightSource = light;
    }
}

void KritiaEngine::Lighting::LightingSystem::Reset() {
    Lights.clear();
    MainLightSource = nullptr;
}
