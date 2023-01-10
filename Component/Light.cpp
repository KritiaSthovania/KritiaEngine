#include "Light.h"
#include "../Component/Transform.h"
#include "../CoreModule/GameObject.h"
#include "../CoreModule/Settings.h"
#include "../Rendering/RenderingProvider.h"
#include "../CoreModule/Lighting.h"

using namespace KritiaEngine::Rendering;

KritiaEngine::Light::Light(GameObject* gameObject)
{
	this->gameObject = gameObject;
	this->color = Color();
	this->type = KritiaEngine::LightType::Directional;
	cachedPosition = Transform()->position;
	cachedRotation = Transform()->rotation;
	UpdateLightMatrices();
	KritiaEngine::Lighting::LightingSystem::AddLight(this);
	RenderingProvider::CreateShadowMap(this);
}

KritiaEngine::Matrix4x4 KritiaEngine::Light::GetLightMatrixVP(int direction) {
	if (cachedPosition != Transform()->position || cachedRotation != Transform()->rotation) {
		cachedPosition = Transform()->position;
		cachedRotation = Transform()->rotation;
		UpdateLightMatrices();
	}
	switch (this->type) {
	case LightType::Directional:
		return directionalLightMatrix;
	case LightType::Point:
		switch (direction) {
		case 0:
			return pointLightMatrixRight;
		case 1:
			return pointLightMatrixLeft;
		case 2:
			return pointLightMatrixTop;
		case 3:
			return pointLightMatrixBottom;
		case 4:
			return pointLightMatrixNear;
		case 5:
			return pointLightMatrixFar;
		default:
			return Matrix4x4::Identity();
		}
	case LightType::Spot:
		return spotLightMatrix;
	default:
		return Matrix4x4::Identity();
	}
}

void KritiaEngine::Light::OnObjectDestroy() {
	Lighting::LightingSystem::RemoveLight(this);
}

void KritiaEngine::Light::UpdateLightMatrices() {

	switch (this->type) {
		case LightType::Directional:
			directionalLightMatrix = Matrix4x4::Ortho(-20, 20, -20, 20, Settings::NearPlaneDistant, Settings::FarPlaneDistance) * Matrix4x4::LookAt(Transform()->position, Transform()->forward, Transform()->up);
			break;
		case LightType::Point:
		{
			Matrix4x4 shadowProj = Matrix4x4::Perspective(90.0f, (float)Settings::ShadowWidth / (float)Settings::ShadowHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistance);
			pointLightMatrixRight = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0));
			pointLightMatrixLeft = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(-1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0));
			pointLightMatrixTop = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, 1.0));
			pointLightMatrixBottom = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, -1.0, 0.0), Vector3(0.0, 0.0, -1.0));
			pointLightMatrixNear = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 0.0, 1.0), Vector3(0.0, -1.0, 0.0));
			pointLightMatrixFar = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 0.0, -1.0), Vector3(0.0, -1.0, 0.0));
			break;
		}
		case LightType::Spot:
		{
			Matrix4x4 shadowProj = Matrix4x4::Perspective(90, (float)Settings::ShadowWidth / (float)Settings::ShadowHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistance);
			spotLightMatrix = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->forward, Transform()->up);
			break;
		}
	}
}
