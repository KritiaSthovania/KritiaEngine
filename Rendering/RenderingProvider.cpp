#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "RenderingProvider.h"
#include "../CoreModule/Settings.h"
#include "../CoreModule/Lighting.h"
#include "../CoreModule/Mathf.h"
#include "../Component/Transform.h"
#include "OpenGLRendering.h"

using namespace KritiaEngine::Rendering;
using namespace KritiaEngine;

bool KritiaEngine::Rendering::RenderingProvider::depthTestEnabled = true;
bool KritiaEngine::Rendering::RenderingProvider::blendEnabled = true;
bool KritiaEngine::Rendering::RenderingProvider::backFaceCullingEnabled = true;
bool RenderingProvider::msaaEnabled = true;
bool RenderingProvider::gammaCorrectionEnabled = true;
unsigned int KritiaEngine::Rendering::RenderingProvider::skyboxVAO = 1;
unsigned int KritiaEngine::Rendering::RenderingProvider::skyboxVBO = 1;
unsigned int KritiaEngine::Rendering::RenderingProvider::uniformBufferIDMatricesVP = 0;
unsigned int RenderingProvider::skyboxTextureID = 0;
std::shared_ptr<Shader> RenderingProvider::skyboxShader = nullptr;
std::vector<Texture> RenderingProvider::skyboxTextures = std::vector<Texture>();

void KritiaEngine::Rendering::RenderingProvider::Initialize() {
	if (Settings::UseOpenGL) {
		OpenGLRendering::Initialize();
		OpenGLRendering::CreateUniformBuffer(&uniformBufferIDMatricesVP, static_cast<unsigned int>(UniformBindingPoint::MatricesVP));
	}
	CreateSkybox();
}

void KritiaEngine::Rendering::RenderingProvider::ClearFramebuffer() {
	if (Settings::UseOpenGL) {
		OpenGLRendering::ClearFramebuffer();
	}
}

void KritiaEngine::Rendering::RenderingProvider::LoadCubeMap(const std::vector<Texture>& cubeTextures, unsigned int* id) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::LoadCubeMap(cubeTextures, id);
	}
}

void KritiaEngine::Rendering::RenderingProvider::CreateSkybox() {
	skyboxShader = std::shared_ptr<Shader>(new Shader("./StandardShader/SkyboxShader.vs", "./StandardShader/SkyboxShader.fs"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/right.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/left.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/top.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/bottom.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/front.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/back.jpg"));
	if (Settings::UseOpenGL) {
		RenderingProvider::LoadCubeMap(skyboxTextures, &skyboxTextureID);
		OpenGLRendering::CreateSkybox(&skyboxVAO, &skyboxVBO, sizeof(skyboxVertices), &skyboxVertices[0]);
	}
}

unsigned int KritiaEngine::Rendering::RenderingProvider::Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel) {
	if (Settings::UseOpenGL) {
		return OpenGLRendering::Load2DTexture(texture, alphaChannel);
	} else {
		return 0;
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderSkybox(Matrix4x4 projection, Matrix4x4 view) {
	if (Settings::UseOpenGL) {	
		OpenGLRendering::RenderSkybox(projection, view);
	}

}

void KritiaEngine::Rendering::RenderingProvider::SetupMesh(const std::shared_ptr<Mesh>& mesh) {
	mesh->isSetup = true;
	mesh->submeshSize = mesh->submeshIndices.size();
	mesh->VAOs.resize(mesh->submeshSize);
	mesh->VBOs.resize(mesh->submeshSize);
	mesh->EBOs.resize(mesh->submeshSize);
	if (Settings::UseOpenGL) {
		OpenGLRendering::SetupMesh(mesh);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::RenderSubmesh(meshFilter, material, submeshIndex, model, viewPos, pos);
	}
}

void KritiaEngine::Rendering::RenderingProvider::UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::UpdateUniformBufferMatricesVP(view, projection);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderGPUInstances(bool transparent) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::RenderGPUInstances(transparent);
	}
}

float KritiaEngine::Rendering::RenderingProvider::skyboxVertices[108] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

