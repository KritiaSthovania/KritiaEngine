#pragma once
#include <pch.h>
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../CoreModule/Mesh.h"
#include "../Component/MeshFilter.h"
#include "../Component/Camera.h"
#include "../CoreModule/Material.h"
#include "../CoreModule/Lighting.h"

namespace KritiaEngine::Rendering {
	class OpenGLRendering {
		friend class RenderingProvider;
	private:
		static void Initialize();
		static void CreateShadowMap(Light* light);
		static void ClearFramebuffer();
		/// <summary>
		/// Load a cube map
		/// </summary>
		/// <param name="cubeTextures">six textures</param>
		/// <param name="id"></param>
		/// <returns>the texture ID</returns>
		static unsigned int LoadCubeMap(const std::vector<Texture>& cubeTextures);
		/// <summary>
		/// Load a 2D texture
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="alphaChannel"></param>
		/// <returns>the texture ID</returns>
		static unsigned int Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel);
		static void RenderSkybox(Matrix4x4 projection, Matrix4x4 view);
		static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		/// <summary>
		/// Shoule be called by RenderingProvider if using OpenGL. The projection and view matrices are set by using the uniform buffer.
		/// </summary>
		/// <param name="model">model matrix</param>
		/// <param name="viewPos">viewing position (e.g. position of the camera)</param>
		/// <param name="pos">position of the object</param>
		/// <param name="shader">shader program of the object</param>
		/// <param name="mainTextureID">mainTextureID of the material</param>
		/// <param name="specularMapID">specularMapID of the material</param>
		static void ApplyMaterialShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Material>& material);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static void RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, Light* light);
		static void SetupRenderShadowMap(Light* light);
		static void SetupRenderSubmesh();
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderGPUInstances(bool transparent);
		/// <summary>
		/// Only consider the main light source.
		/// </summary>
		static void CreateUniformBuffer(unsigned int bindingPoint);
		static void CreateSkybox(const std::vector<Texture>& skyboxTextures, unsigned int verticesSize, float* verticesPos);
		static void SetMainLightProperties(const std::shared_ptr<Shader>& shader);
		static void SetPointLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void SetSpotLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void UpdateGPUInstancingCount(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, int submeshIndex, Matrix4x4 model);
		static void UpdateGPUInstancingBuffer();
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingCount;
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingBufferIDs;
		static std::map<std::tuple<Mesh, Material, int>, std::vector<Matrix4x4>> gpuInstancingMatrices;
		static unsigned int skyboxVAO, skyboxVBO;
		static unsigned int skyboxTextureID;
		static std::shared_ptr<Shader> skyboxShader, shadowMapShader, shadowMapShaderPoint;
		static unsigned int uniformBufferIDMatricesVP;
	};
}


