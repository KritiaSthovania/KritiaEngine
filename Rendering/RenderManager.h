#pragma once
#include <pch.h>
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "Shader.h"

namespace KritiaEngine {
	static class RenderManager {	
	public:
		static void Initialize();
		static void ClearFramebuffer();
		static void LoadCubeMap(std::vector<std::shared_ptr<Texture>> skyBoxTextures, unsigned int* id);
		/// <summary>
		/// Load a 2D texture and return the id, call LoadCubeMap if you want to load a cube map.
		/// </summary>
		/// <param name="texture"></param>
		static unsigned int Load2DTexture(std::shared_ptr<Texture> texture, bool alphaChannel);
		static void RenderSkybox(unsigned int skyboxTexutureID, Matrix4x4 projection, Matrix4x4 view, std::shared_ptr<Shader> skyboxShader);
		static void ApplyMaterialShaderOnRender(const Matrix4x4& projection, const Matrix4x4& view, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos, std::shared_ptr<Shader> shader,
			                                    unsigned int mainTextureID, unsigned int specularMapID);
		static bool depthTestEnabled;
		static bool blendEnabled;
		static bool backFaceCullingEnabled;
	private:
		static void CreateSkybox();
		static void OpenGLInitialize();
		static void OpenGLClearFramebuffer();
		static void SetMainLightProperties(std::shared_ptr<Shader> shader);
		static void SetPointLightProperties(const Vector3& pos, std::shared_ptr<Shader> shader);
		static void SetSpotLightProperties(const Vector3& pos, std::shared_ptr<Shader> shader);
		static float skyboxVertices[108];
		static unsigned int skyboxVAO, skyboxVBO;
	};
}


