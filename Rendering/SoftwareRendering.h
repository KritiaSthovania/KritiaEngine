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
			Vector4 ScreenPosition;
			Vector4 NDC;
			Vector3 Normal;
			Vector3 FragPos;
			Vector3 FragPosLightSpace;
			Vector2 TexCoord;
			Vector3 T;
			Vector3 B;
			Vector3 N;
		};

		struct FragmentShadingFields {

		};
		//static std::vector<std::vector<Mesh::Vertex>> vertexBuffer;
		//static std::vector<std::vector<int>> vertexIndexBuffer;
		static Matrix4x4 viewMatrix;
		static Matrix4x4 projectionMatrix;
		static std::vector<std::vector<Color>> screenBuffer;
		static std::vector<std::vector<float>> depthBuffer;

		static void Initialize();
		//static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		static void UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		static ShadingInOutFields VertexShading(const Mesh::Vertex& vertex, const Matrix4x4& model, const Matrix3x3 &normalMatrix, Vector4& screenPos);
		static void Rasterize(int startIndex, const std::vector<ShadingInOutFields>& vertexOutFields, std::vector<ShadingInOutFields>& fragmentInFields);
		static Vector2 ViewportTransform(const Vector4& ndc);
		static bool InTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c);
		static std::pair<Vector2, Color> FragmentShading(const std::shared_ptr<Material>& material, const std::vector<ShadingInOutFields>& inFields);
		static void DrawPixel(const Vector2& position, const Color& color);
	};
}



