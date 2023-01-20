#include "Material.h"
#include "Lighting.h"
#include "Settings.h"
#include <stb_image.h>
#include "../Component/Transform.h"
#include "../Rendering/RenderingProvider.h"

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
			if (renderMode == Transparent) {
				mainTextureID = RenderingProvider::Load2DTexture(mainTexture, true);

			} else if (renderMode == Opaque) {
				mainTextureID = RenderingProvider::Load2DTexture(mainTexture, false);
			}
		}
		if (specularMap != nullptr) {
			if (renderMode == Transparent) {
				specularMapID = RenderingProvider::Load2DTexture(specularMap, true);

			} else if (renderMode == Opaque) {
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
		shader->SetVec3("albedo", albedo.RGB());
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



