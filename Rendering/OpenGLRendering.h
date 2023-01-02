#pragma once
#include <pch.h>
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../CoreModule/Mesh.h"
#include "../Component/MeshFilter.h"
#include "../Component/Camera.h"
#include "../CoreModule/Material.h"



namespace KritiaEngine::Rendering {
	class OpenGLRendering {
		friend class RenderingProvider;
	private:
		static void Initialize();
		static void ClearFramebuffer();
		static void LoadCubeMap(const std::vector<Texture>& cubeTextures, unsigned int* id);
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
		static void ApplyMaterialShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos, const std::shared_ptr<Shader>& shader, unsigned int mainTextureID, unsigned int specularMapID);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderGPUInstances(bool transparent);
		static void CreateUniformBuffer(unsigned int* id, unsigned int bindingPoint);
		static void CreateSkybox(unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int verticesSize, float* verticesPos);
		static void SetMainLightProperties(const std::shared_ptr<Shader>& shader);
		static void SetPointLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void SetSpotLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void UpdateGPUInstancingCount(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, int submeshIndex, Matrix4x4 model);
		static void UpdateGPUInstancingBuffer();
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingCount;
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingBufferIDs;
		static std::map<std::tuple<Mesh, Material, int>, std::vector<Matrix4x4>> gpuInstancingMatrices;
	};
}


