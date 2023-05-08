#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "RenderingProvider.h"
#include "../CoreModule/Settings.h"
#include "../CoreModule/Lighting.h"
#include "../CoreModule/Mathf.h"
#include "../Component/Transform.h"
#include "OpenGLRendering.h"
#include "SoftwareRendering.h"

using namespace KritiaEngine::Rendering;
using namespace KritiaEngine;

bool KritiaEngine::Rendering::RenderingProvider::depthTestEnabled = true;
bool KritiaEngine::Rendering::RenderingProvider::blendEnabled = true;
bool KritiaEngine::Rendering::RenderingProvider::backFaceCullingEnabled = true;
bool RenderingProvider::msaaEnabled = true;
bool RenderingProvider::gammaCorrectionEnabled = true;
bool RenderingProvider::shadowEnabled = true;
std::vector<std::shared_ptr<Texture>> RenderingProvider::skyboxTextures = std::vector<std::shared_ptr<Texture>>();
Matrix4x4 RenderingProvider::projection;
RenderingProvider::RenderingBackend RenderingProvider::backend = RenderingProvider::RenderingBackend::Software;

void KritiaEngine::Rendering::RenderingProvider::Initialize(HWND hwnd) {
	if (Settings::UseOpenGL) {
		backend = RenderingBackend::OpenGL;
		OpenGLRendering::Initialize();
		OpenGLRendering::CreateUniformBuffer(static_cast<unsigned int>(UniformBindingPoint::MatricesVP));
	} else if (Settings::UseSoftwareRendering) {
		backend = RenderingBackend::Software;
		SoftwareRendering::Initialize(hwnd);
	}
	CreateSkybox();
}

void KritiaEngine::Rendering::RenderingProvider::ClearFramebuffer() {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::ClearFramebuffer();
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::ClearFramebuffer();
	}
}

void KritiaEngine::Rendering::RenderingProvider::SwapFramebuffer(GLFWwindow* window) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::SwapFramebuffer(window);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::SwapFramebuffer();
	}
}

unsigned int KritiaEngine::Rendering::RenderingProvider::LoadCubeMap(const std::vector<std::shared_ptr<Texture>>& cubeTextures) {
	if (backend == RenderingBackend::OpenGL) {
		return OpenGLRendering::LoadCubeMap(cubeTextures);
	} else if (backend == RenderingBackend::Software){
		return SoftwareRendering::LoadCubeMap(cubeTextures);
	}
}

void KritiaEngine::Rendering::RenderingProvider::CreateShadowMap(Light* light) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::CreateShadowMap(light);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::CreateShadowMap(light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::CreateSkybox() {
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/right.jpg")));
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/left.jpg")));
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/top.jpg")));
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/bottom.jpg")));
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/front.jpg")));
	skyboxTextures.push_back(std::make_shared<Texture>(Texture("./Assets/Textures/skybox/back.jpg")));
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::CreateSkybox(skyboxTextures, sizeof(skyboxVertices), &skyboxVertices[0]);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::CreateSkybox(skyboxTextures, sizeof(skyboxVertices), &skyboxVertices[0]);
	}
}

void KritiaEngine::Rendering::RenderingProvider::Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::Load2DTexture(texture, alphaChannel);
	} else if (backend == RenderingBackend::Software){
		SoftwareRendering::Load2DTexture(texture, alphaChannel);
	}
}

unsigned int KritiaEngine::Rendering::RenderingProvider::Load2DTextureFromPath(const std::string& path, bool alphaChannel, Vector2& size, int& channel) {
	if (backend == RenderingBackend::OpenGL) {
		return OpenGLRendering::Load2DTextureFromPath(path, alphaChannel, size, channel);
	} else if (backend == RenderingBackend::Software){
		return SoftwareRendering::Load2DTexture(path, alphaChannel, size, channel);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderSkybox(Matrix4x4 projection, Matrix4x4 view) {
	if (backend == RenderingBackend::OpenGL) {	
		OpenGLRendering::RenderSkybox(projection, view);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::RenderSkybox(projection, view);
	}
}

void KritiaEngine::Rendering::RenderingProvider::SetupMesh(const std::shared_ptr<Mesh>& mesh) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::SetupMesh(mesh);
	} else if (backend == RenderingBackend::Software) {

	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::RenderSubmesh(meshFilter, material, submeshIndex, model, viewPos, pos);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::RenderSubmesh(meshFilter, material, submeshIndex, model, viewPos, pos);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, Light* light) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::RenderShadowMap(meshFilter, submeshIndex, model, light);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::RenderShadowMap(meshFilter, submeshIndex, model, light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection) {
	RenderingProvider::projection = projection;
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::UpdateUniformBufferMatricesVP(view, projection);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::UpdateUniformBufferMatricesVP(view, projection);
	}
}

void KritiaEngine::Rendering::RenderingProvider::RenderGPUInstances(bool transparent) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::RenderGPUInstances(transparent);
	} else if (backend == RenderingBackend::Software) {
		
	}
}

void KritiaEngine::Rendering::RenderingProvider::SetupRenderShadowMap(Light* light) {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::SetupRenderShadowMap(light);
	} else if (backend == RenderingBackend::Software) {
		SoftwareRendering::SetupRenderShadowMap(light);
	}
}

void KritiaEngine::Rendering::RenderingProvider::SetupRenderSubmesh() {
	if (backend == RenderingBackend::OpenGL) {
		OpenGLRendering::SetupRenderSubmesh();
	} else if (backend == RenderingBackend::Software) {

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

