#pragma once
#include "../CoreModule/Texture.h"
#include "../CoreModule/MathStructs.h"
#include "../CoreModule/Mesh.h"
#include "../Component/MeshFilter.h"
#include "../Component/Camera.h"
#include "../CoreModule/Material.h"
#include "../CoreModule/Lighting.h"
#include "../CoreModule/Utilities.h"
#include <pch.h>

namespace KritiaEngine::Rendering {

	class SoftwareRendering {
		friend class RenderingProvider;
	private:
		// corresponding to the out/in variables in the BlinnPhongShader
		struct ShadingInOutFields {
			Vector4 WorldPosition;
			Vector4 NDC;
			Vector3 Normal;
			Vector3 FragPos;
			Vector4 FragPosLightSpace;
			Vector2 TexCoord;
			Vector3 T;
			Vector3 B;
			Vector3 N;
			Vector2 ScreenPosition;
		};

		//static std::vector<std::vector<Mesh::Vertex>> vertexBuffer;
		//static std::vector<std::vector<int>> vertexIndexBuffer;
		static Matrix4x4 viewMatrix;
		static Matrix4x4 projectionMatrix;
		static std::vector<std::vector<Color>> frameBuffer;
		static std::vector<std::vector<float>> depthBuffer;
		static std::map<unsigned int, std::vector<std::vector<Color>>> shadowFramebuffers;
		static std::map<unsigned int, std::shared_ptr<Texture>> shadowMap;
		static std::map<unsigned int, std::vector<std::shared_ptr<Texture>>> shadowMapPoint;
		static const int pointLightPcfSamples = 20;
		static Vector3 sampleOffsetDirections[pointLightPcfSamples];

		static void Initialize();
		static void CreateShadowMap(Light* light);
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static ShadingInOutFields VertexShading(const Mesh::Vertex& vertex, const Matrix4x4& model, const Matrix3x3 &normalMatrix, Vector4& screenPos);
		static void Rasterize(int startIndex, const std::vector<ShadingInOutFields>& vertexOutFields, std::vector<ShadingInOutFields>& fragmentInFields);
		static Vector2 ViewportTransform(const Vector4& ndc);
		static bool InTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c);
		static void FragmentShading(const std::shared_ptr<Material>& material, const std::vector<ShadingInOutFields>& inFields, const Vector3& viewPos, const Vector3& pos);
		static Color SampleTexture(const std::shared_ptr<Texture>& texture, const Vector2& texCoord);
		static Color SampleCubeTexture(const std::vector<std::shared_ptr<Texture>>& textures, const Vector3& direction);
		static Vector2 ComputeParallaxMapping(const std::shared_ptr<Material>& material, const Vector2& texCoord, const Vector3& viewDir);
		static float ComputeMainShadow(const std::shared_ptr<Material>& material, const Vector4& fragPosLightSpace);
		static Color ComputePointLight(const std::shared_ptr<Material>& material, Light* pointLight, const Vector3& normal, const Vector3& fragPos, const Vector3& viewDir, const Vector2& texCoord, const Vector3& albedo, const ShadingInOutFields& inField, const Vector3& viewPos);
		static Color ComputeSpotLight(const std::shared_ptr<Material>& material, Light* spotLight, const Vector3& normal, const Vector3& fragPos, const Vector3& viewDir, const Vector2& texCoord, const Vector3& albedo, const ShadingInOutFields& inField);
		static float ComputePointShadow(const std::shared_ptr<Material>& material, Light* pointLight, const ShadingInOutFields& inField, const Vector3& viewPos);
		static float ComputeSpotShadow(const std::shared_ptr<Material>& material, Light* spotLight, const ShadingInOutFields& inField);
		static void DrawPixel(const Vector2& position, const Color& color);
	};
}



