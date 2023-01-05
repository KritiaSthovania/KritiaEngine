#pragma once
#include <pch.h>
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../Component/MeshFilter.h"
#include "../CoreModule/Material.h"
#include "../CoreModule/Lighting.h"
using namespace KritiaEngine;

namespace KritiaEngine::Rendering{
	class RenderingProvider {	
		friend class OpenGLRendering;
	public:
		enum class UniformBindingPoint {
			MatricesVP = 0
		};
		static void Initialize();
		static void ClearFramebuffer();
		static void LoadCubeMap(const std::vector<Texture>& cubeTextures, unsigned int* id);
		/// <summary>
		/// Load a 2D texture and return the id, call LoadCubeMap if you want to load a cube map.
		/// </summary>
		/// <param name="texture"></param>
		static unsigned int Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel);
		static void RenderSkybox(Matrix4x4 projection, Matrix4x4 view);
		static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static void RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, const std::shared_ptr<Light>& light);
		/// <summary>
		/// Update the uniform buffer "MatricesVP" in the order of view and projection
		/// </summary>
		/// <param name="view">the view matrix</param>
		/// <param name="projection">the projection matrix</param>
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderGPUInstances(bool transparent);
		static void SetupRenderShadowMap();
		static void SetupRenderSubmesh();
		static unsigned int GetShadowMapID();
		static bool depthTestEnabled;
		static bool blendEnabled;
		static bool backFaceCullingEnabled;
		static bool msaaEnabled;
		static bool gammaCorrectionEnabled;
		static bool shadowEnabled;
	private:
		static void CreateShadowMap();
		static void CreateSkybox();
		static float skyboxVertices[108];
		static std::vector<Texture> skyboxTextures;
	};
}



