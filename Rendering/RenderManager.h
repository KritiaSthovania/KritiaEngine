#pragma once
#include <pch.h>
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../CoreModule/Mesh.h"

namespace KritiaEngine {
	static class RenderManager {	
	public:
		enum UniformBindingPoint {
			MatricesVP = 0
		};
		static void Initialize();
		static void ClearFramebuffer();
		static void LoadCubeMap(std::vector<std::shared_ptr<Texture>> skyBoxTextures, unsigned int* id);
		/// <summary>
		/// Load a 2D texture and return the id, call LoadCubeMap if you want to load a cube map.
		/// </summary>
		/// <param name="texture"></param>
		static unsigned int Load2DTexture(std::shared_ptr<Texture> texture, bool alphaChannel);
		static void RenderSkybox(unsigned int skyboxTexutureID, Matrix4x4 projection, Matrix4x4 view, std::shared_ptr<Shader> skyboxShader);
		/// <summary>
		/// Shoule be called by Material::ApplyShderOnRender if using OpenGL. The projection and view matrices are set by using the uniform buffer.
		/// </summary>
		/// <param name="model">model matrix</param>
		/// <param name="viewPos">viewing position (e.g. position of the camera)</param>
		/// <param name="pos">position of the object</param>
		/// <param name="shader">shader program of the object</param>
		/// <param name="mainTextureID">mainTextureID of the material</param>
		/// <param name="specularMapID">specularMapID of the material</param>
		static void ApplyMaterialShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos, std::shared_ptr<Shader> shader, unsigned int mainTextureID, unsigned int specularMapID);
		static void SetupMesh(std::shared_ptr<Mesh> mesh);
		static void RenderSubmesh(std::shared_ptr<Mesh> mesh, int submeshIndex);
		/// <summary>
		/// Update the uniform buffer "MatricesVP" in the order of view and projection
		/// </summary>
		/// <param name="view">the view matrix</param>
		/// <param name="projection">the projection matrix</param>
		static void UpdateUniformBufferMatricesVP(Matrix4x4 view, Matrix4x4 projection);
		static bool depthTestEnabled;
		static bool blendEnabled;
		static bool backFaceCullingEnabled;
		static unsigned int uniformBufferIDMatricesVP;
	private:
		static void CreateUniformBuffer(unsigned int* id, UniformBindingPoint bindingPoint);
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


