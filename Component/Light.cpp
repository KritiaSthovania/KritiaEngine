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
	cachedPosition = Vector3::Zero();
	cachedRotation = Quaternion::Identity();
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
	Component::OnObjectDestroy();
	Lighting::LightingSystem::RemoveLight(this);
}

void KritiaEngine::Light::UpdateLightMatrices() {
	// Directional
	directionalLightMatrix = Matrix4x4::Ortho(-10, 10, -10, 10, Settings::NearPlaneDistance, Settings::FarPlaneDistance) * Matrix4x4::LookAt(Transform()->position, Transform()->position + Transform()->forward, Transform()->up);
	// Point
	Matrix4x4 shadowProj = Matrix4x4::Perspective(90.0f, (float)Settings::ShadowWidth / (float)Settings::ShadowHeight, Settings::NearPlaneDistance, Settings::FarPlaneDistance);
	pointLightMatrixRight = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0));
	pointLightMatrixLeft = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(-1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0));
	pointLightMatrixTop = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, 1.0));
	pointLightMatrixBottom = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, -1.0, 0.0), Vector3(0.0, 0.0, -1.0));
	pointLightMatrixNear = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 0.0, 1.0), Vector3(0.0, -1.0, 0.0));
	pointLightMatrixFar = shadowProj * Matrix4x4::LookAt(Transform()->position, Transform()->position + Vector3(0.0, 0.0, -1.0), Vector3(0.0, -1.0, 0.0));
	// Spot
	Matrix4x4 shadowProjSpot = Matrix4x4::Perspective(90, (float)Settings::ShadowWidth / (float)Settings::ShadowHeight, Settings::NearPlaneDistance, Settings::FarPlaneDistance);
	spotLightMatrix = shadowProjSpot * Matrix4x4::LookAt(Transform()->position, Transform()->forward, Transform()->up);
}

void KritiaEngine::Light::OnInspector() {
	const char* preview;
	switch (type) {
	case LightType::Directional:
		preview = "Directional";
		break;
	case LightType::Spot:
		preview = "Spot";
		break;
	case LightType::Point:
		preview = "Point";
		break;
	default:
		preview = "Directional";
		break;
	}
	ImGui::Text("Type");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Type", preview)) {
		if (ImGui::Selectable("Directional")) {
			type = LightType::Directional;
		}
		if (ImGui::Selectable("Spot")) {
			type = LightType::Spot;
		}
		if (ImGui::Selectable("Point")) {
			type = LightType::Point;
		}
		ImGui::EndCombo();
	}
	ImguiAlias::ColorField3("Color", &color.r);
	ImguiAlias::BoolField("Casting Shadow", &castingShadow);
	if (ImGui::TreeNodeEx("Intensity", ImguiAlias::treeFlagDefaultOpenAndSpanFullWidth)) {
		ImguiAlias::FloatField("Ambient", &ambientIntensity);
		ImguiAlias::FloatField("Diffuse", &diffuseIntensity);
		ImguiAlias::FloatField("Specular", &specularIntensity);
		ImGui::TreePop();
	}
	if (type == LightType::Point || type == LightType::Spot) {
		if (ImGui::TreeNodeEx("Point/Spot Light Attenuation", ImGuiTreeNodeFlags_SpanFullWidth)) {
			ImguiAlias::FloatField("Constant Factor", &constantAttenuationFactor);
			ImguiAlias::FloatField("Linear Factor", &linearAttenuationFactor);
			ImguiAlias::FloatField("Quadratic Factor", &quadraticAttenuationFactor);
			ImGui::TreePop();
		}
	}
	if (type == LightType::Spot) {
		if (ImGui::TreeNodeEx("Spot Light Cutoff Angle", ImGuiTreeNodeFlags_SpanFullWidth)){
			ImguiAlias::FloatField("Inner", &cutOffAngleInner);
			ImguiAlias::FloatField("Outer", &cutOffAngleOuter);
			ImGui::TreePop();
		}
	}
}

std::string KritiaEngine::Light::SerializeToJson() {
	json json;
	const char* preview;
	switch (type) {
	case LightType::Directional:
		preview = "Directional";
		break;
	case LightType::Spot:
		preview = "Spot";
		break;
	case LightType::Point:
		preview = "Point";
		break;
	default:
		preview = "Directional";
		break;
	}
	json["Type"] = "Light";
	json["LightType"] = preview;
	json["Color"] = { color.r, color.g, color.b, color.a };
	json["CastingShadow"] = castingShadow;
	json["Ambient"] = ambientIntensity;
	json["Diffuse"] = diffuseIntensity;
	json["Specular"] = specularIntensity;
	json["ConstantFactor"] = constantAttenuationFactor;
	json["LinearFactor"] = linearAttenuationFactor;
	json["QuadraticFactor"] = quadraticAttenuationFactor;
	json["InnerCutoff"] = cutOffAngleInner;
	json["OuterCutoff"] = cutOffAngleOuter;
	return json.dump();
}

void KritiaEngine::Light::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "Light");
	if (json["LightType"] == "Spot") {
		type = LightType::Spot;
	} else if (json["LightType"] == "Point") {
		type = LightType::Point;
	} else {
		type = LightType::Directional;
	}
	color.r = json["Color"][0];
	color.g = json["Color"][1];
	color.b = json["Color"][2];
	color.a = json["Color"][3];
	castingShadow = json["CastingShadow"];
	ambientIntensity = json["Ambient"];
	diffuseIntensity = json["Diffuse"];
	specularIntensity = json["Specular"];
	constantAttenuationFactor = json["ConstantFactor"];
	linearAttenuationFactor = json["LinearFactor"];
	quadraticAttenuationFactor = json["QuadraticFactor"];
	cutOffAngleInner = json["InnerCutoff"];
	cutOffAngleOuter = json["OuterCutoff"];
}

std::string KritiaEngine::Light::GetInspectorLabel() {
	return inspectorLabel;
}
