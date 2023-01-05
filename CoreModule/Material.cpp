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
			shader->SetInt("mainTexture", diffuseSamplerIndex);
		}
		if (specularMap != nullptr) {
			if (renderMode == Transparent) {
				specularMapID = RenderingProvider::Load2DTexture(specularMap, true);

			} else if (renderMode == Opaque) {
				specularMapID = RenderingProvider::Load2DTexture(specularMap, false);
			}
			shader->SetInt("specularMap", specularSamplerIndex);
		}
		shader->SetInt("shadowMap", shadowSamplerIndex);
		shader->SetFloat("shininess", shininess);
		shader->SetVec3("albedo", albedo.RGB());
		// Make sure the shader supports GPU instancing while using it.
		if (GPUInstancingEnabled) {
			shader->SetBool("instancing", GPUInstancingEnabled);
		}
		initialized = true;
	}
}



