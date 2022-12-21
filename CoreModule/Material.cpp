#include "Material.h"
#include "Lighting.h"
#include "Settings.h"
#include <stb_image.h>
#include "../Component/Transform.h"
#include "../Rendering/RenderManager.h"

using namespace KritiaEngine;

KritiaEngine::Material::Material()
{
	name = "New Material";
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlingPhongShader.vs", "./StandardShader/BlingPhongShader.fs"));
}

KritiaEngine::Material::Material(const char* name)
{
	this->name = name;
	shader = std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlingPhongShader.vs", "./StandardShader/BlingPhongShader.fs"));
}

KritiaEngine::Material::Material(const char* name, std::shared_ptr<Shader> shader)
{
	this->name = name;
	this->shader = shader;
}

void KritiaEngine::Material::ApplyShaderOnRender(const Matrix4x4& model, const Vector3 &viewPos, const Vector3 &pos)
{
	RenderManager::ApplyMaterialShaderOnRender(model, viewPos, pos, shader, mainTextureID, specularMapID);
}

void Material::Initialize() {
	shader->Use();
	shader->UniformBlockBinding(shader->GetUniformBlockIndex("MatricesVP"), RenderManager::UniformBindingPoint::MatricesVP);
	if (mainTexture != nullptr) {
		if (renderMode == Transparent) {
			mainTextureID = RenderManager::Load2DTexture(mainTexture, true);

		} else if (renderMode == Opaque) {
			mainTextureID = RenderManager::Load2DTexture(mainTexture, false);
		}
		shader->SetInt("mainTexture", diffuseSamplerIndex);
	}
	if (specularMap != nullptr) {
		if (renderMode == Transparent) {
			specularMapID = RenderManager::Load2DTexture(specularMap, true);

		} else if (renderMode == Opaque) {
			specularMapID = RenderManager::Load2DTexture(specularMap, false);
		}
		shader->SetInt("specularMap", specularSamplerIndex);
	}
	shader->SetFloat("shininess", shininess);
	shader->SetVec3("albedo", albedo.RGB());
}



