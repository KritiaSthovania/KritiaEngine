#include "Material.h"
#include "Lighting.h"
#include "Settings.h"
#include "../Component/Transform.h"
#include "../Rendering/RenderingProvider.h"
#include <stb_image.h>
#include <fstream>

using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;

KritiaEngine::Material::Material()
{
	name = "New Material";
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlinnPhongShader.vs", "./StandardShader/BlinnPhongShader.fs"));
}

KritiaEngine::Material::Material(const char* name)
{
	this->name = name;
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlinnPhongShader.vs", "./StandardShader/BlinnPhongShader.fs"));
}

KritiaEngine::Material::Material(const char* name, const std::shared_ptr<Shader>& shader)
{
	this->name = name;
	this->shader = shader;
}

void Material::Initialize() {
	if (!initialized) {
		shader->Use();
		shader->UniformBlockBinding(shader->GetUniformBlockIndex("MatricesVP"), static_cast<unsigned int>(RenderingProvider::UniformBindingPoint::MatricesVP));
		if (mainTexture != nullptr) {
			if (renderMode == RenderMode::Transparent) {
				mainTextureID = RenderingProvider::Load2DTexture(mainTexture, true);

			} else if (renderMode == RenderMode::Opaque) {
				mainTextureID = RenderingProvider::Load2DTexture(mainTexture, false);
			}
		}
		if (specularMap != nullptr) {
			if (renderMode == RenderMode::Transparent) {
				specularMapID = RenderingProvider::Load2DTexture(specularMap, true);

			} else if (renderMode == RenderMode::Opaque) {
				specularMapID = RenderingProvider::Load2DTexture(specularMap, false);
			}
		}
		if (normalMap != nullptr) {
			normalMapID = RenderingProvider::Load2DTexture(normalMap, false);
			shader->SetBool("hasNormalMap", true);
		} else {
			shader->SetBool("hasNormalMap", false);
		}
		if (parallaxMap != nullptr) {
			parallaxMapID = RenderingProvider::Load2DTexture(parallaxMap, false);
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
		if (GPUInstancingEnabled) {
			shader->SetBool("instancing", GPUInstancingEnabled);
		}
		initialized = true;
	}
}

std::string KritiaEngine::Material::SerializeToJson() {
	json json;
	json["Type"] = "Material";
	if (renderMode == RenderMode::Opaque) {
		json["RenderMode"] = "Opaque";
	} else if( renderMode == RenderMode::Transparent) {
		json["RenderMode"] = "Transparent";
	}
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

void KritiaEngine::Material::DeserializeFromJson(const json& json) {
	assert(json["Type"] == "Material");
	if (json["RenderMode"] == "Opaque") {
		this->renderMode = RenderMode::Opaque;
	} else if (json["RenderMode"] == "Transparent") {
		this->renderMode = RenderMode::Transparent;
	}
	albedo = Color(json["Albedo"][0], json["Albedo"][1], json["Albedo"][2], 1);
	shininess = json["Shininess"];
	GPUInstancingEnabled = json["GPUInstancingEnabled"];
	if (json["MainTexture"] != "Null") {
		mainTexture = std::shared_ptr<Texture>(new Texture());
		nlohmann::ordered_json mainTexJson = json::parse((std::string)json["MainTexture"]);
		mainTexture->DeserializeFromJson(mainTexJson);
	}
	if (json["SpecularMap"] != "Null") {
		specularMap = std::shared_ptr<Texture>(new Texture());
		nlohmann::ordered_json specJson = json::parse((std::string)json["SpecularMap"]);
		specularMap->DeserializeFromJson(specJson);
	}
	if (json["NormalMap"] != "Null") {
		normalMap = std::shared_ptr<Texture>(new Texture());
		nlohmann::ordered_json normalJson = json::parse((std::string)json["NormalMap"]);
		normalMap->DeserializeFromJson(normalJson);
	}
	if (json["ParallaxMap"] != "Null") {
		parallaxMap = std::shared_ptr<Texture>(new Texture());
		nlohmann::ordered_json paraJson = json::parse((std::string)json["ParallexMap"]);
		parallaxMap->DeserializeFromJson(paraJson);
	}
}

void KritiaEngine::Material::DeserializeFromPath(const std::string& path) {
	std::ifstream instream(path);
	json json = json::parse(instream);
	DeserializeFromJson(json);
	instream.close();
}



