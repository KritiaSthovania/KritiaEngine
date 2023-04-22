#include "SoftwareRendering.h"
#include "../CoreModule/Settings.h"
#include "../CoreModule/Mathf.h"
using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;

Matrix4x4 SoftwareRendering::viewMatrix;
Matrix4x4 SoftwareRendering::projectionMatrix;
std::vector<std::vector<Color>> SoftwareRendering::screenBuffer;
std::vector<std::vector<float>> SoftwareRendering::depthBuffer;

void KritiaEngine::Rendering::SoftwareRendering::Initialize() {
	screenBuffer.resize(Settings::ScreenWidth);
	for (std::vector<Color> column : screenBuffer) {
		column.resize(Settings::ScreenHeight);
	}
	for (int i = 0; i < Settings::ScreenWidth; i++) {
		depthBuffer.push_back(std::vector<float>());
		for (int j = 0; j < Settings::ScreenHeight; j++) {
			depthBuffer[i].push_back(-1);
		}
	}
}

void KritiaEngine::Rendering::SoftwareRendering::UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection) {
	viewMatrix = view;
	projectionMatrix = projection;
}

void KritiaEngine::Rendering::SoftwareRendering::RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos) {
	auto mesh = meshFilter->mesh;
	std::vector<ShadingInOutFields> vertexOutFields; 
	std::vector<ShadingInOutFields> fragmentInFields;
	Matrix3x3 normalMatrix = Matrix3x3({ model.GetEntry(0, 0), model.GetEntry(1, 0), model.GetEntry(2, 0),
									 model.GetEntry(0, 1), model.GetEntry(1, 1), model.GetEntry(2, 1),
									 model.GetEntry(0, 2), model.GetEntry(1, 2), model.GetEntry(2, 2) }).Inverse().Transpose();
	for (int i = 0; i < mesh->submeshVertices[submeshIndex].size(); i++) {
		Vector4 screenPos;
		vertexOutFields.push_back(VertexShading(mesh->submeshVertices[submeshIndex][i], model, normalMatrix, screenPos));
	}
	for (int i = 0; i < mesh->submeshIndices[submeshIndex][i] - 2; i++) {
		Rasterize(i, vertexOutFields, fragmentInFields);
	}
	for (int i = 0; i < vertexOutFields.size(); i++) {
		FragmentShading(material, fragmentInFields);
	}

	//for (std::pair<Vector2, Color> p : pixelsToDraw) {
	//	DrawPixel(p.first, p.second);
	//}
}

SoftwareRendering::ShadingInOutFields SoftwareRendering::VertexShading(const Mesh::Vertex& vertex, const Matrix4x4& model, const Matrix3x3& normalMatrix, Vector4& screenPos) {
	ShadingInOutFields out;
	out.ScreenPosition = Vector4(projectionMatrix * viewMatrix * model * Vector4(vertex.Position, 1.0));
	out.NDC = Vector4(out.ScreenPosition.x / out.ScreenPosition.w, out.ScreenPosition.y / out.ScreenPosition.w, out.ScreenPosition.z / out.ScreenPosition.w, out.ScreenPosition.w / out.ScreenPosition.w);
	out.FragPos = Vector3(model * Vector4(vertex.Position, 1.0));
	out.Normal = normalMatrix * vertex.Normal;
	out.TexCoord = vertex.TexCoord;
	out.FragPosLightSpace = Lighting::LightingSystem::GetMainLightSource()->GetLightMatrixVP(0) * Vector4(out.FragPos, 1.0);
	// Gram-Schmidt Process
	out.T = Vector3::Normalize(Vector3(model * Vector4(vertex.Tangent, 0.0)));
	out.N = Vector3::Normalize(Vector3(model * Vector4(vertex.Normal, 0.0)));
	// re-orthogonalize T wrt. N
	out.T = Vector3::Normalize(out.T - Vector3::Dot(out.T, out.N) * out.N);
	// retrieve perpendicular vector B with the cross product of T and N
	out.B = Vector3::Cross(out.T, out.N);
	return out;
}

void KritiaEngine::Rendering::SoftwareRendering::Rasterize(int startIndex, const std::vector<ShadingInOutFields>& vertexOutFields, std::vector<ShadingInOutFields>& fragmentInFields) {
	Vector2 pixelSize = Vector2(1.f / Settings::ScreenWidth, 1.f / Settings::ScreenHeight);
	Vector2 screenPos1 = ViewportTransform(vertexOutFields[startIndex].NDC);
	Vector2 screenPos2 = ViewportTransform(vertexOutFields[startIndex + 1].NDC);
	Vector2 screenPos3 = ViewportTransform(vertexOutFields[startIndex + 2].NDC);
	float minX = std::min(screenPos1.x, screenPos2.x, screenPos3.x);
	float maxX = std::max(screenPos1.x, screenPos2.x, screenPos3.x);
	float minY = std::min(screenPos1.y, screenPos2.y, screenPos3.y);
	float maxY = std::max(screenPos1.y, screenPos2.y, screenPos3.y);
	// Pixel is always on positions with an integer subscript, so we iterate over integers
	for (int i = minX; i <= maxX; i++) {
		for (int j = minY; j <= maxY; j++) {
			if (InTriangle(Vector2(i + pixelSize.x / 2, j + pixelSize.y / 2), screenPos1, screenPos2, screenPos3)) {
				// For all pixels in the triangle, we interpolate the vertex and get one ShadingInOutFields for fragment shading
				float lambda12 = std::abs(Vector2::Cross(screenPos1 - screenPos2, Vector2(i, j) - screenPos2) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda13 = std::abs(Vector2::Cross(screenPos3 - screenPos1, Vector2(i, j) - screenPos1) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda23 = std::abs(Vector2::Cross(screenPos2 - screenPos3, Vector2(i, j) - screenPos3) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				ShadingInOutFields in;
				float z1 = vertexOutFields[startIndex].ScreenPosition.z;
				float z2 = vertexOutFields[startIndex + 1].ScreenPosition.z;
				float z3 = vertexOutFields[startIndex + 2].ScreenPosition.z;
				float z4 = 1 / (lambda12 / z3 + lambda13 / z2 + lambda23 / z1);
				in.ScreenPosition = z4 * (lambda12 * vertexOutFields[startIndex + 2].ScreenPosition / z3 + lambda13 * vertexOutFields[startIndex + 1].ScreenPosition / z2 + lambda23 * vertexOutFields[startIndex].ScreenPosition / z1);
				in.NDC = z4 * (lambda12 * vertexOutFields[startIndex + 2].NDC / z3 + lambda13 * vertexOutFields[startIndex + 1].NDC / z2 + lambda23 * vertexOutFields[startIndex].NDC / z1);
				in.FragPos = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPos / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPos / z2 + lambda23 * vertexOutFields[startIndex].FragPos / z1);
				in.FragPosLightSpace = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPosLightSpace / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPosLightSpace / z2 + lambda23 * vertexOutFields[startIndex].FragPosLightSpace / z1);
				in.Normal = z4 * (lambda12 * vertexOutFields[startIndex + 2].Normal / z3 + lambda13 * vertexOutFields[startIndex + 1].Normal / z2 + lambda23 * vertexOutFields[startIndex].Normal / z1);
				in.TexCoord = z4 * (lambda12 * vertexOutFields[startIndex + 2].TexCoord / z3 + lambda13 * vertexOutFields[startIndex + 1].TexCoord / z2 + lambda23 * vertexOutFields[startIndex].TexCoord / z1);
				in.T = z4 * (lambda12 * vertexOutFields[startIndex + 2].T / z3 + lambda13 * vertexOutFields[startIndex + 1].T / z2 + lambda23 * vertexOutFields[startIndex].T / z1);
				in.N = z4 * (lambda12 * vertexOutFields[startIndex + 2].N / z3 + lambda13 * vertexOutFields[startIndex + 1].N / z2 + lambda23 * vertexOutFields[startIndex].N / z1);
				in.B = z4 * (lambda12 * vertexOutFields[startIndex + 2].B / z3 + lambda13 * vertexOutFields[startIndex + 1].B / z2 + lambda23 * vertexOutFields[startIndex].B / z1);
				fragmentInFields.push_back(in);
			}
		}
	}
}

Vector2 ViewportTransform(const Vector4& ndc) {
	return Vector2(ndc.x * Settings::ScreenWidth / 2 + Settings::ScreenWidth / 2, ndc.y * Settings::ScreenHeight / 2 + Settings::ScreenHeight / 2);
}

bool InTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c) {
	Vector2 ab = b - a;
	Vector2 bc = c - b;
	Vector2 ca = a - c;
	if (Vector2::Cross(bc, p - b) * Vector2::Cross(bc, a - b) > 0 && Vector2::Cross(ca, p - c) * Vector2::Cross(ca, b - c) > 0 && Vector2::Cross(ab, p - a) * Vector2::Cross(ab, c - a) > 0) {
		return true;
	} else {
		return false;
	}
}

std::pair<Vector2, Color> KritiaEngine::Rendering::SoftwareRendering::FragmentShading(const std::shared_ptr<Material>& material, const std::vector<ShadingInOutFields>& inFields) {
	

}


void KritiaEngine::Rendering::SoftwareRendering::DrawPixel(const Vector2& position, const Color& color) {
	glBegin(GL_POINT);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(position.x, position.y);
	glEnd();
}
