#include "Light.h"
#include "../Component/Transform.h"
#include "../CoreModule/GameObject.h"
#include "../CoreModule/Settings.h"

KritiaEngine::Light::Light(GameObject* gameObject)
{
	this->gameObject = gameObject;
	this->color = Color();
	this->type = Directional;
	this->lightSpaceMatrixVP = Matrix4x4::Ortho(-20, 20, -20, 20, Settings::NearPlaneDistant, Settings::FarPlaneDistant) * Matrix4x4::LookAt(Transform()->Position, Transform()->Position + direction, Vector3(0, 1, 0));
}

KritiaEngine::Matrix4x4 KritiaEngine::Light::GetLightMatrixVP() {
	if (this->type == Directional) {
		lightSpaceMatrixVP = Matrix4x4::Ortho(-20, 20, -20, 20, Settings::NearPlaneDistant, Settings::FarPlaneDistant) * Matrix4x4::LookAt(Transform()->Position, Transform()->Position + direction, Vector3(0, 1, 0));
	}
	return lightSpaceMatrixVP;
}
