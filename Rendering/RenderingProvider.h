#pragma once
#include <pch.h>
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../CoreModule/Mesh.h"
#include "../Component/MeshFilter.h"
#include "../Component/Camera.h"

namespace KritiaEngine {
	class RenderingProvider {	
	public:
		enum UniformBindingPoint {
			MatricesVP = 0
		};
		static void Initialize();
		static void ClearFramebuffer();
		static void LoadCubeMap(const std::vector<Texture>& skyboxTextures, unsigned int* id);
		/// <summary>
		/// Load a 2D texture and return the id, call LoadCubeMap if you want to load a cube map.
		/// </summary>
		/// <param name="texture"></param>
		static unsigned int Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel);
		static void RenderSkybox(Matrix4x4 projection, Matrix4x4 view);
		/// <summary>
		/// Shoule be called by Material::ApplyShderOnRender if using OpenGL. The projection and view matrices are set by using the uniform buffer.
		/// </summary>
		/// <param name="model">model matrix</param>
		/// <param name="viewPos">viewing position (e.g. position of the camera)</param>
		/// <param name="pos">position of the object</param>
		/// <param name="shader">shader program of the object</param>
		/// <param name="mainTextureID">mainTextureID of the material</param>
		/// <param name="specularMapID">specularMapID of the material</param>
		static void ApplyMaterialShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos, const std::shared_ptr<Shader>& shader, unsigned int mainTextureID, unsigned int specularMapID);
		static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		/// <summary>
		/// Update the uniform buffer "MatricesVP" in the order of view and projection
		/// </summary>
		/// <param name="view">the view matrix</param>
		/// <param name="projection">the projection matrix</param>
		static void UpdateUniformBufferMatricesVP(Matrix4x4 view, Matrix4x4 projection);
		static void RenderGPUInstances(bool transparent);

		static bool depthTestEnabled;
		static bool blendEnabled;
		static bool backFaceCullingEnabled;
		static unsigned int uniformBufferIDMatricesVP;
		friend bool operator< (const std::tuple<Mesh, Material, int>& left, const std::tuple<Mesh, Material, int>& right);
	private:
		static void CreateUniformBuffer(unsigned int* id, UniformBindingPoint bindingPoint);
		static void CreateSkybox();
		static void OpenGLInitialize();
		static void OpenGLClearFramebuffer();
		static void SetMainLightProperties(const std::shared_ptr<Shader>& shader);
		static void SetPointLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void SetSpotLightProperties(const Vector3& pos, const std::shared_ptr<Shader>& shader);
		static void UpdateGPUInstancingCount(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, int submeshIndex, Matrix4x4 model);
		static void UpdateGPUInstancingBuffer();
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingCount;
		static std::map<std::tuple<Mesh, Material, int>, unsigned int> gpuInstancingBufferIDs;
		static std::map<std::tuple<Mesh, Material, int>, std::vector<Matrix4x4>> gpuInstancingMatrices;
		static float skyboxVertices[108];
		static unsigned int skyboxVAO, skyboxVBO;
		static unsigned int skyboxTextureID;
		static std::shared_ptr<Shader> skyboxShader;
		static std::vector<Texture> skyboxTextures;
	};
}


