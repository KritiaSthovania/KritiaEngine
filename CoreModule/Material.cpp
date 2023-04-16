#include "Material.h"
#include "Lighting.h"
#include "Settings.h"
#include "../Component/Transform.h"
#include "../Rendering/RenderingProvider.h"
#include "../Editor/EditorApplication.h"
#include "Manager/ResourceManager.h"
#include <imgui/imgui.h>
#include <stb_image.h>
#include <fstream>

using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;
using namespace KritiaEngine::Editor;
using namespace KritiaEngine::Manager;

KritiaEngine::Material::Material()
{
	name = "Default Diffuse Material";
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader((EditorApplication::currentProjectFolderPath + "/StandardShader/BlinnPhongShader.vs").c_str(), (EditorApplication::currentProjectFolderPath + "/StandardShader/BlinnPhongShader.fs").c_str()));
}

KritiaEngine::Material::Material(const char* name)
{
	this->name = name;
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader((EditorApplication::currentProjectFolderPath + "/StandardShader/BlinnPhongShader.vs").c_str(), (EditorApplication::currentProjectFolderPath + "/StandardShader/BlinnPhongShader.fs").c_str()));
}

KritiaEngine::Material::Material(const char* name, const std::shared_ptr<Shader>& shader)
{
	this->name = name;
	this->shader = shader;
}

KritiaEngine::Material::Material(const std::shared_ptr<Shader>& shader) {
	this->name = "Default Diffuse Material";
	this->shader = shader;
}

void Material::Initialize() {
	if (!initialized) {
		shader->Use();
		shader->UniformBlockBinding(shader->GetUniformBlockIndex("MatricesVP"), static_cast<unsigned int>(RenderingProvider::UniformBindingPoint::MatricesVP));
		if (mainTexture != nullptr) {
			if (renderMode == RenderMode::Transparent) {
				RenderingProvider::Load2DTexture(mainTexture, true);

			} else if (renderMode == RenderMode::Opaque) {
				RenderingProvider::Load2DTexture(mainTexture, false);
			}
		}
		if (specularMap != nullptr) {
			if (renderMode == RenderMode::Transparent) {
				RenderingProvider::Load2DTexture(specularMap, true);

			} else if (renderMode == RenderMode::Opaque) {
				RenderingProvider::Load2DTexture(specularMap, false);
			}
		}
		if (normalMap != nullptr) {
			RenderingProvider::Load2DTexture(normalMap, false);
			shader->SetBool("hasNormalMap", true);
		} else {
			shader->SetBool("hasNormalMap", false);
		}
		if (parallaxMap != nullptr) {
			RenderingProvider::Load2DTexture(parallaxMap, false);
			shader->SetBool("hasParallaxMap", true);
			shader->SetFloat("heightScale", 0.1f);
			shader->SetInt("depthLayers", 10);
		} else {
			shader->SetBool("hasParallaxMap", false);
		}
		shader->SetInt("mainTexture", diffuseSamplerIndex);
		shader->SetInt("specularMap", specularSamplerIndex);
		shader->SetInt("normalMap", normalSamplerIndex);
		shader->SetInt("parallaxMap", parallaxSamplerIndex);
		shader->SetInt("shadowMap", shadowSamplerIndex);
		shader->SetFloat("shininess", shininess);
		shader->SetVec3("albedo", albedo.GetRGB());
		for (int i = 0; i < Lighting::LightingSystem::MaxPointLightsForOneObject; i++) {
			std::string str = "pointLights[" + std::to_string(i) + "]" + ".shadowMapCube";
			shader->SetInt(str, shadowSamplerIndex + 1 + i);
		}
		for (int i = 0; i < Lighting::LightingSystem::MaxSpotLightsForOneObject; i++) {
			std::string str = "spotLights[" + std::to_string(i) + "]" + ".shadowMapSpot";
			shader->SetInt(str, shadowSamplerIndex + 1 + Lighting::LightingSystem::MaxPointLightsForOneObject + i);
		}
		shader->SetFloat("farPlaneDistance", Settings::FarPlaneDistance);
		// Make sure the shader supports GPU instancing while using it.
		shader->SetBool("instancing", GPUInstancingEnabled);
		initialized = true;
	}
}

void KritiaEngine::Material::SetPropertiesOnRender() {
	shader->Use();
	shader->SetFloat("shininess", shininess);
	shader->SetVec3("albedo", albedo.GetRGB());
	shader->SetFloat("farPlaneDistance", Settings::FarPlaneDistance);
	shader->SetBool("instancing", GPUInstancingEnabled);
}

std::string KritiaEngine::Material::SerializeToJson() {
	json json;
	json["Type"] = "Material";
	json["Name"] = name;
	if (guid == "") {
		GenerateGuid();
	}
	json["Guid"] = guid;
	if (renderMode == RenderMode::Opaque) {
		json["RenderMode"] = "Opaque";
	} else if( renderMode == RenderMode::Transparent) {
		json["RenderMode"] = "Transparent";
	}
	json["Path"] = path;
	json["Albedo"] = { albedo.r, albedo.g, albedo.b };
	json["Shininess"] = shininess;
	json["GPUInstancingEnabled"] = GPUInstancingEnabled;
	json["MainTexture"] = mainTexture == nullptr ? "Null" : mainTexture->SerializeToJson();
	json["SpecularMap"] = specularMap == nullptr ? "Null" : specularMap->SerializeToJson();
	json["NormalMap"] = normalMap == nullptr ? "Null" : normalMap->SerializeToJson();
	json["ParallaxMap"] = parallaxMap == nullptr ? "Null" : parallaxMap->SerializeToJson();
	json["Shader"] = shader == nullptr ? "Null" : shader->SerializeToJson();
	return json.dump();
}

std::shared_ptr<Material> KritiaEngine::Material::DeserializeFromJson(const json& json) {
	std::shared_ptr<Material> mat = std::make_shared<Material>(Material());
	assert(json["Type"] == "Material");
	if (json["RenderMode"] == "Opaque") {
		mat->renderMode = RenderMode::Opaque;
	} else if (json["RenderMode"] == "Transparent") {
		mat->renderMode = RenderMode::Transparent;
	}
	mat->name = json["Name"];
	mat->guid = json["Guid"];
	mat->path = json["Path"];
	mat->albedo = Color(json["Albedo"][0], json["Albedo"][1], json["Albedo"][2], 1);
	mat->shininess = json["Shininess"];
	mat->GPUInstancingEnabled = json["GPUInstancingEnabled"];
	if (json["MainTexture"] != "Null") {
		nlohmann::ordered_json mainTexJson = json::parse((std::string)json["MainTexture"]);
		mat->mainTexture = ResourceManager::GetTexture(mainTexJson);
	}
	if (json["SpecularMap"] != "Null") {
		nlohmann::ordered_json specJson = json::parse((std::string)json["SpecularMap"]);
		mat->specularMap = ResourceManager::GetTexture(specJson);
	}
	if (json["NormalMap"] != "Null") {
		nlohmann::ordered_json normalJson = json::parse((std::string)json["NormalMap"]);
		mat->normalMap = ResourceManager::GetTexture(normalJson);
	}
	if (json["ParallaxMap"] != "Null") {
		nlohmann::ordered_json paraJson = json::parse((std::string)json["ParallexMap"]);
		mat->parallaxMap = ResourceManager::GetTexture(paraJson);
	}
	return mat;
}

void KritiaEngine::Material::SerializeToFile() {
	std::string path = ImguiAlias::OpenSaveResourceWindow("Material", KritiaEngine::Editor::materialFilePostfix, "New Material");
	if (!path.ends_with(KritiaEngine::Editor::materialFilePostfix)) {
		path += ("/" + (std::string)KritiaEngine::Editor::materialFilePostfix);
	}
	this->path = path;
	std::string jsonStr = SerializeToJson();
	std::fstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	output << jsonStr << std::endl;
	output.close();
}

void KritiaEngine::Material::OnInspector() {
	ImGui::Text(((std::string)"Material   " + name).c_str());
	ImGui::Separator();
	const char* renderModeStr = "";
	switch (renderMode) {
	case RenderMode::Opaque:
		renderModeStr = "Opaque";
		break;
	case RenderMode::Transparent:
		renderModeStr = "Transparent";
		break;
	}
	ImGui::Text("RenderMode");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##RenderMode", renderModeStr)) {
		if (ImGui::Selectable("Opaque")) {
			renderMode = RenderMode::Opaque;
		}
		if (ImGui::Selectable("Transparent")) {
			renderMode = RenderMode::Transparent;
		}
		ImGui::EndCombo();
	}
	ImguiAlias::ColorField3("Albedo", &albedo.r);
	ImguiAlias::FloatField("Shininess", &shininess);
	ImguiAlias::BoolField("GPUInstancingEnabled", &GPUInstancingEnabled);
	// TODO: Drag And Drop
	ImGui::Text("Main Texture ");
	ImGui::SameLine();
	if(ImGui::Button(mainTexture == nullptr? "Null" : mainTexture->name.c_str())) {
		KritiaEngine::Editor::GUI::ImguiManager::currentSelectedInspectable = mainTexture.get();
	}
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		std::string path = ImguiAlias::OpenFindResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix);
		if (path != "") {
			mainTexture = ResourceManager::GetTexture(path);
			if (renderMode == RenderMode::Transparent) {
				RenderingProvider::Load2DTexture(mainTexture, true);

			} else if (renderMode == RenderMode::Opaque) {
				RenderingProvider::Load2DTexture(mainTexture, false);
			}
		}
	}
	ImGui::Text("Specular Map ");
	ImGui::SameLine();
	if (ImGui::Button(specularMap == nullptr ? "Null" : specularMap->name.c_str())) {
		KritiaEngine::Editor::GUI::ImguiManager::currentSelectedInspectable = specularMap.get();
	}
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		std::string path = ImguiAlias::OpenFindResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix);
		if (path != "") {
			specularMap = ResourceManager::GetTexture(path);
			if (renderMode == RenderMode::Transparent) {
				RenderingProvider::Load2DTexture(specularMap, true);

			} else if (renderMode == RenderMode::Opaque) {
				RenderingProvider::Load2DTexture(specularMap, false);
			}
		}
	}
	ImGui::Text("Normal Map   ");
	ImGui::SameLine();
	if (ImGui::Button(normalMap == nullptr ? "Null" : normalMap->name.c_str())) {
		KritiaEngine::Editor::GUI::ImguiManager::currentSelectedInspectable = normalMap.get();
	}
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		std::string path = ImguiAlias::OpenFindResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix);
		if (path != "") {
			normalMap = ResourceManager::GetTexture(path);
			RenderingProvider::Load2DTexture(normalMap, false);
			shader->SetBool("hasNormalMap", true);
		}
	}
	ImGui::Text("Parallax Map ");
	ImGui::SameLine();
	if (ImGui::Button(parallaxMap == nullptr ? "Null" : parallaxMap->name.c_str())) {
		KritiaEngine::Editor::GUI::ImguiManager::currentSelectedInspectable = parallaxMap.get();
	}
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		std::string path = ImguiAlias::OpenFindResourceWindow("Texture", KritiaEngine::Editor::textureFilePostfix);
		if (path != "") {
			parallaxMap = ResourceManager::GetTexture(path);
			RenderingProvider::Load2DTexture(parallaxMap, false);
			shader->SetBool("hasParallaxMap", true);
			shader->SetFloat("heightScale", 0.1f);
			shader->SetInt("depthLayers", 10);
		}
	}
}
