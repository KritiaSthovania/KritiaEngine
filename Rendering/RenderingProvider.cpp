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
bool RenderingProvider::shadowEnabled = true;
std::vector<Texture> RenderingProvider::skyboxTextures = std::vector<Texture>();
Matrix4x4 RenderingProvider::projection;

void KritiaEngine::Rendering::RenderingProvider::Initialize() {
	if (Settings::UseOpenGL) {
		OpenGLRendering::Initialize();
		OpenGLRendering::CreateUniformBuffer(static_cast<unsigned int>(UniformBindingPoint::MatricesVP));
	}
	CreateSkybox();
}

void KritiaEngine::Rendering::RenderingProvider::ClearFramebuffer() {
	if (Settings::UseOpenGL) {
		OpenGLRendering::ClearFramebuffer();
	}
}

unsigned int KritiaEngine::Rendering::RenderingProvider::LoadCubeMap(const std::vector<Texture>& cubeTextures) {
	if (Settings::UseOpenGL) {
		return OpenGLRendering::LoadCubeMap(cubeTextures);
	} else {
		return 0;
	}
}

void KritiaEngine::Rendering::RenderingProvider::CreateShadowMap(Light* light) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::CreateShadowMap(light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::CreateSkybox() {
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/right.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/left.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/top.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/bottom.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/front.jpg"));
	skyboxTextures.push_back(Texture("./Assets/Textures/skybox/back.jpg"));
	if (Settings::UseOpenGL) {
		OpenGLRendering::CreateSkybox(skyboxTextures, sizeof(skyboxVertices), &skyboxVertices[0]);
	}
}

void KritiaEngine::Rendering::RenderingProvider::Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::Load2DTexture(texture, alphaChannel);
	} else {
	}
}

unsigned int KritiaEngine::Rendering::RenderingProvider::Load2DTextureFromPath(const std::string& path, bool alphaChannel) {
	if (Settings::UseOpenGL) {
		return OpenGLRendering::Load2DTextureFromPath(path, alphaChannel);
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
	if (Settings::UseOpenGL) {
		OpenGLRendering::SetupMesh(mesh);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::RenderSubmesh(meshFilter, material, submeshIndex, model, viewPos, pos);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, Light* light) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::RenderShadowMap(meshFilter, submeshIndex, model, light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection) {
	RenderingProvider::projection = projection;
	if (Settings::UseOpenGL) {
		OpenGLRendering::UpdateUniformBufferMatricesVP(view, projection);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderGPUInstances(bool transparent) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::RenderGPUInstances(transparent);
	}
}

void KritiaEngine::Rendering::RenderingProvider::SetupRenderShadowMap(Light* light) {
	if (Settings::UseOpenGL) {
		OpenGLRendering::SetupRenderShadowMap(light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::SetupRenderSubmesh() {
	if (Settings::UseOpenGL) {
		OpenGLRendering::SetupRenderSubmesh();
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

