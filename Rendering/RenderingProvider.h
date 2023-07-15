#pragma once
#include <vector>
#include <pch.h>
#include "../CoreModule/MathStructs.h"
#include "Shader.h"
#include "../Component/MeshFilter.h"
#include "../CoreModule/Material.h"
#include "../CoreModule/Lighting.h"
#include <windows.h>
using namespace KritiaEngine;

namespace KritiaEngine::Rendering{
	class RenderingProvider {	
		friend class OpenGLRendering;
	public:
		enum class UniformBindingPoint {
			MatricesVP = 0
		};
		enum class RenderingBackend {
			OpenGL,
			Vulkan,
			Software
		};

		static void Initialize(HWND hwnd, GLFWwindow* window);
		static void Cleanup();
		static void SetupRenderingFrame();
		static void EndRenderingFrame(GLFWwindow* window);
		/// <summary>
		/// Load a cube map
		/// </summary>
		/// <param name="cubeTextures">six textures</param>
		/// <param name="id"></param>
		/// <returns>the texture ID</returns>
		static unsigned int LoadCubeMap(const std::vector<std::shared_ptr<Texture>>& cubeTextures);
		/// <summary>
		/// Load a 2D texture and return the id, call LoadCubeMap if you want to load a cube map.
		/// </summary>
		/// <param name="texture"></param>
		static void Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel);
		static unsigned int Load2DTextureFromPath(const std::string& path, bool alphaChannel, Vector2& size, int& channel);
		static void RenderSkybox(Matrix4x4 projection, Matrix4x4 view);
		static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static void RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, Light* light);
		/// <summary>
		/// Update the uniform buffer "MatricesVP" in the order of view and projection
		/// </summary>
		/// <param name="view">the view matrix</param>
		/// <param name="projection">the projection matrix</param>
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderGPUInstances(bool transparent);
		static void SetupRenderShadowMap(Light* light);
		static void SetupRenderSubmesh();
		static void CreateShadowMap(Light* light);
		static void InitializeMaterial(Material* material);
		static void InitializeMeshFilter(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material);
		static bool depthTestEnabled;
		static bool blendEnabled;
		static bool backFaceCullingEnabled;
		static bool msaaEnabled;
		static bool gammaCorrectionEnabled;
		static bool shadowEnabled;
		static Matrix4x4 projection;
	private:

		static void CreateSkybox();
		static float skyboxVertices[108];
		static std::vector<std::shared_ptr<Texture>> skyboxTextures;
		static RenderingBackend backend;
	};
}



