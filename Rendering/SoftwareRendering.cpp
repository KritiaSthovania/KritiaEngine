#include "SoftwareRendering.h"
#include "../Component/Transform.h"
#include "../CoreModule/Settings.h"
#include "../CoreModule/Mathf.h"
#include <stb/stb_image.h>
using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;
using namespace KritiaEngine::Lighting;

Matrix4x4 SoftwareRendering::viewMatrix;
Matrix4x4 SoftwareRendering::projectionMatrix;
std::vector<std::vector<Color>> SoftwareRendering::frameBuffer;
std::vector<std::vector<float>> SoftwareRendering::depthBuffer;
std::map<unsigned int, std::vector<std::vector<float>>> SoftwareRendering::shadowFramebuffers;
std::map<unsigned int, std::shared_ptr<Texture>>  SoftwareRendering::shadowMap;
std::map<unsigned int, std::vector<std::shared_ptr<Texture>>> SoftwareRendering::shadowMapPoint;
std::map<unsigned int, unsigned char*> SoftwareRendering::textures;
unsigned int SoftwareRendering::texture2DCounter = 0;
unsigned int SoftwareRendering::shadowMapBufferCounter = 0;
HDC SoftwareRendering::dc;

Vector3 SoftwareRendering::sampleOffsetDirections[pointLightPcfSamples] = { Vector3(1, 1, 1), Vector3(1, -1, 1), Vector3(-1, -1, 1), Vector3(-1, 1, 1),
		   Vector3(1, 1, -1), Vector3(1, -1, -1), Vector3(-1, -1, -1), Vector3(-1, 1, -1),
		   Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0), Vector3(-1, 1, 0),
		   Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
		   Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, -1, -1), Vector3(0, 1, -1) };

void SoftwareRendering::Initialize(HWND hwnd) {
	dc = GetWindowDC(hwnd);
	for (int i = 0; i < Settings::ScreenWidth; i++) {
		frameBuffer.push_back(std::vector<Color>());
		for (int j = 0; j < Settings::ScreenWidth; j++) {
			frameBuffer[i].push_back(Color(0, 0, 0, 1));
		}
	}
	for (int i = 0; i < Settings::ScreenWidth; i++) {
		depthBuffer.push_back(std::vector<float>());
		for (int j = 0; j < Settings::ScreenHeight; j++) {
			depthBuffer[i].push_back(FLT_MAX);
		}
	}
}

void SoftwareRendering::ClearFramebuffer() {
	for (std::vector<Color>& column : frameBuffer) {
		for (Color c : column) {
			c = Color(0, 0, 0, 1);
		}
	}
}

void KritiaEngine::Rendering::SoftwareRendering::SwapFramebuffer() {
	for (int i = 0; i < frameBuffer.size(); i++) {
		for (int j = 0; j < frameBuffer[i].size(); j++) {
			DrawPixel(Vector2(i, j), frameBuffer[i][j]);
		}
	}
	//for (int i = 0; i < shadowFramebuffers[0].size(); i++) {
	//	for (int j = 0; j < shadowFramebuffers[0][i].size(); j++) {
	//		if (shadowFramebuffers[0][i][j] < 1 && shadowFramebuffers[0][i][j]>0) {
	//			shadowFramebuffers[0][i][j] *= 50;
	//			float x = shadowFramebuffers[0][i][j];
	//		}
	//		DrawPixel(Vector2(i, j), Color(shadowFramebuffers[0][i][j], 0, 0, 1));
	//	}
	//}
}

void KritiaEngine::Rendering::SoftwareRendering::CreateShadowMap(Light* light) {
	std::shared_ptr<Texture> shadowMap = std::make_shared<Texture>(Texture());
	shadowMap->loaded = true;
	light->shadowMapFBO = shadowMapBufferCounter;
	shadowMapBufferCounter++;
	light->shadowMapID = texture2DCounter;
	texture2DCounter++;
	shadowFramebuffers[light->shadowMapFBO] = std::vector<std::vector<float>>();
	shadowFramebuffers[light->shadowMapFBO].resize(Settings::ShadowWidth);
	for (int i = 0; i < shadowFramebuffers[light->shadowMapFBO].size(); i++) {
		shadowFramebuffers[light->shadowMapFBO][i] = std::vector<float>();
		shadowFramebuffers[light->shadowMapFBO][i].resize(Settings::ShadowHeight);
		for (int j = 0; j < shadowFramebuffers[light->shadowMapFBO][i].size(); j++) {
			shadowFramebuffers[light->shadowMapFBO][i][j] = 1;
		}
	}
}


void SoftwareRendering::UpdateUniformBufferMatricesVP(const Matrix4x4& view, const Matrix4x4& projection) {
	viewMatrix = view;
	projectionMatrix = projection;
}

void SoftwareRendering::Load2DTexture(const std::shared_ptr<Texture>& texture, bool alphaChannel) {
	if (!texture->loaded) {
		unsigned int id = texture2DCounter;
		texture2DCounter++;
		int width;
		int height;
		int nrChannels;
		if (alphaChannel) {
			textures[id] = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 4);
			if (textures[id]) {
				texture->size = Vector2(width, height);
				texture->channels = 4;
			} else {
				std::cout << "Failed to load texture at " << texture->path << std::endl;
			}
		} else {
			textures[id] = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 3);
			if (textures[id]) {
				texture->size = Vector2(width, height);
				texture->channels = 3;
			} else {
				std::cout << "Failed to load texture at " << texture->path << std::endl;
			}
		}
		texture->ID = id;
		texture->loaded = true;
	}
}

unsigned int KritiaEngine::Rendering::SoftwareRendering::Load2DTexture(const std::string& path, bool alphaChannel, Vector2& size, int& channel) {
	unsigned int id = texture2DCounter;
	texture2DCounter++;
	int width;
	int height;
	int nrChannels;
	if (alphaChannel) {
		textures[id] = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
		if (textures[id]) {
			size.x = width;
			size.y = height;
			channel = 4;
		} else {
			std::cout << "Failed to load texture at " << path.c_str() << std::endl;
		}
	} else {
		textures[id] = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
		if (textures[id]) {
			size.x = width;
			size.y = height;
			channel = 3;
		} else {
			std::cout << "Failed to load texture at " << path.c_str() << std::endl;
		}
	}
	return id;
}

void KritiaEngine::Rendering::SoftwareRendering::SetupRenderShadowMap(Light* light) {
	for (int i = 0; i < shadowFramebuffers[light->shadowMapFBO].size(); i++) {
		shadowFramebuffers[light->shadowMapFBO][i] = std::vector<float>();
		shadowFramebuffers[light->shadowMapFBO][i].resize(Settings::ShadowHeight);
		for (int j = 0; j < shadowFramebuffers[light->shadowMapFBO][i].size(); j++) {
			shadowFramebuffers[light->shadowMapFBO][i][j] = 1;
		}
	}
}

void SoftwareRendering::RenderShadowMap(const std::shared_ptr<MeshFilter>& meshFilter, int submeshIndex, const Matrix4x4& model, Light* light) {
	if (light->type == LightType::Directional || light->type == LightType::Spot) {
		auto mesh = meshFilter->mesh;
		std::vector<ShadowShadingInOutFields> shadowVertexOut;
		std::vector<ShadowShadingInOutFields> shadowFragmentIn;
		for (int i = 0; i < mesh->submeshVertices[submeshIndex].size(); i++) {
			VertexShadingShadow(mesh->submeshVertices[submeshIndex][i], light->GetLightMatrixVP(0), model, shadowVertexOut);
		}
		for (int i = 0; i < shadowVertexOut.size() - 2; i++) {
			RasterizeShadow(i, shadowVertexOut, shadowFragmentIn);
		}
		FragmentShadingShadow(shadowFragmentIn, light);
	} else {
		//shadowMapShaderPoint->Use();
		//shadowMapShaderPoint->SetMat4("shadowMatrices[0]", light->GetLightMatrixVP(0));
		//shadowMapShaderPoint->SetMat4("shadowMatrices[1]", light->GetLightMatrixVP(1));
		//shadowMapShaderPoint->SetMat4("shadowMatrices[2]", light->GetLightMatrixVP(2));
		//shadowMapShaderPoint->SetMat4("shadowMatrices[3]", light->GetLightMatrixVP(3));
		//shadowMapShaderPoint->SetMat4("shadowMatrices[4]", light->GetLightMatrixVP(4));
		//shadowMapShaderPoint->SetMat4("shadowMatrices[5]", light->GetLightMatrixVP(5));
		//shadowMapShaderPoint->SetMat4("model", model);
		//shadowMapShaderPoint->SetFloat("far_plane", Settings::FarPlaneDistance);
		//shadowMapShaderPoint->SetVec3("lightPos", light->Transform()->position);
		//glBindVertexArray(meshFilter->mesh->VAOs[submeshIndex]);
		//glDrawElements(GL_TRIANGLES, meshFilter->mesh->submeshIndices[submeshIndex].size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
	}
}

void SoftwareRendering::VertexShadingShadow(const Mesh::Vertex& vertex, const Matrix4x4& lightSpaceMatrix, const Matrix4x4& model, std::vector<ShadowShadingInOutFields>& shadowVertexOut) {
	Vector4 lightSpacePos = lightSpaceMatrix * model * Vector4(vertex.Position, 1.f);
	ShadowShadingInOutFields out;
	out.LightSpacePosition = lightSpacePos;
	if (lightSpacePos.w > 0) {
		// flip y?
		out.NDC = Vector4(lightSpacePos.x / lightSpacePos.w, lightSpacePos.y / lightSpacePos.w, lightSpacePos.z / lightSpacePos.w, lightSpacePos.w / lightSpacePos.w);
	}
	shadowVertexOut.push_back(out);
}
  
void SoftwareRendering::RasterizeShadow(int startIndex, const std::vector<ShadowShadingInOutFields>& shadowVertexOut, std::vector<ShadowShadingInOutFields>& shadowFragmentIn) {
	const Vector2 pixelSize = Vector2(1.f / Settings::ShadowWidth, 1.f / Settings::ShadowHeight);
	Vector2 screenPos1 = ViewportTransformShadow(shadowVertexOut[startIndex].NDC);
	Vector2 screenPos2 = ViewportTransformShadow(shadowVertexOut[startIndex + 1].NDC);
	Vector2 screenPos3 = ViewportTransformShadow(shadowVertexOut[startIndex + 2].NDC);
	float minX = Mathf::Min({ screenPos1.x, screenPos2.x, screenPos3.x });
	float maxX = Mathf::Max({ screenPos1.x, screenPos2.x, screenPos3.x });
	float minY = Mathf::Min({ screenPos1.y, screenPos2.y, screenPos3.y });
	float maxY = Mathf::Max({ screenPos1.y, screenPos2.y, screenPos3.y });
	// Pixel is always on positions with an integer subscript, so we iterate over integers
	for (int i = Mathf::Max((int)minX, 0); i < (int)Mathf::Min((int)maxX, Settings::ShadowWidth); i++) {
		for (int j = Mathf::Max((int)minY, 0); j < (int)Mathf::Min((int)maxY, Settings::ShadowHeight); j++) {
			if (InTriangle(Vector2(i + pixelSize.x / 2, j + pixelSize.y / 2), screenPos1, screenPos2, screenPos3)) {
				// For all pixels in the triangle, we interpolate the vertex and get one ShadingInOutFields for fragment shading
				float lambda12 = std::abs(Vector2::Cross(screenPos1 - screenPos2, Vector2(i, j) - screenPos2) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda13 = std::abs(Vector2::Cross(screenPos3 - screenPos1, Vector2(i, j) - screenPos1) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda23 = std::abs(Vector2::Cross(screenPos2 - screenPos3, Vector2(i, j) - screenPos3) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				ShadowShadingInOutFields in;
				float z1 = shadowVertexOut[startIndex].LightSpacePosition.z;
				float z2 = shadowVertexOut[startIndex + 1].LightSpacePosition.z;
				float z3 = shadowVertexOut[startIndex + 2].LightSpacePosition.z;
				float z4 = 1 / (lambda12 / z3 + lambda13 / z2 + lambda23 / z1);
				in.LightSpacePosition = z4 * (lambda12 * shadowVertexOut[startIndex + 2].LightSpacePosition / z3 + lambda13 * shadowVertexOut[startIndex + 1].LightSpacePosition / z2 + lambda23 * shadowVertexOut[startIndex].LightSpacePosition / z1);
				in.NDC = z4 * (lambda12 * shadowVertexOut[startIndex + 2].NDC / z3 + lambda13 * shadowVertexOut[startIndex + 1].NDC / z2 + lambda23 * shadowVertexOut[startIndex].NDC / z1);
				in.ScreenPosition = Vector2(i, j);
				shadowFragmentIn.push_back(in);
			}
		}
	}
}

Vector2 KritiaEngine::Rendering::SoftwareRendering::ViewportTransformShadow(const Vector4& ndc) {
	return Vector2(ndc.x * Settings::ShadowWidth / 2 + Settings::ShadowWidth / 2, ndc.y * Settings::ShadowHeight / 2 + Settings::ShadowHeight / 2);
}

void SoftwareRendering::FragmentShadingShadow(const std::vector<ShadowShadingInOutFields>& inFields, Light* light) {
	for (int i = 0; i < inFields.size(); i++) {
		ShadowShadingInOutFields in = inFields[i];
		float depth = inFields[i].NDC.z * 0.5 + 0.5;
		if (depth > shadowFramebuffers[light->shadowMapFBO][(int)in.ScreenPosition.x][(int)in.ScreenPosition.y]) {
			continue;
		} else {
			Vector3 depth = (Vector3)inFields[i].NDC * 0.5 + Vector3(0.5, 0.5, 0.5);
			shadowFramebuffers[light->shadowMapFBO][(int)in.ScreenPosition.x][(int)in.ScreenPosition.y] = depth.z;
		}	
	}
}

void SoftwareRendering::SetupRenderSubmesh() {
	for (int i = 0; i < Settings::ScreenWidth; i++) {
		depthBuffer.push_back(std::vector<float>());
		for (int j = 0; j < Settings::ScreenHeight; j++) {
			depthBuffer[i].push_back(FLT_MAX);
		}
	}
}

void SoftwareRendering::RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos) {
	auto mesh = meshFilter->mesh;
	std::vector<ShadingInOutFields> vertexOutFields;
	std::vector<ShadingInOutFields> fragmentInFields;
	Matrix3x3 normalMatrix = Matrix3x3({ model.GetEntry(0, 0), model.GetEntry(1, 0), model.GetEntry(2, 0),
									 model.GetEntry(0, 1), model.GetEntry(1, 1), model.GetEntry(2, 1),
									 model.GetEntry(0, 2), model.GetEntry(1, 2), model.GetEntry(2, 2) }).Inverse().Transpose();
	for (int i = 0; i < mesh->submeshVertices[submeshIndex].size(); i++) {
		Vector4 screenPos;
		VertexShading(mesh->submeshVertices[submeshIndex][i], model, normalMatrix, screenPos, vertexOutFields);
	}
	for (int i = 0; i < vertexOutFields.size() - 2; i++) {
		Rasterize(i, vertexOutFields, fragmentInFields);
	}
	FragmentShading(material, fragmentInFields, viewPos, pos);
	
}

void SoftwareRendering::VertexShading(const Mesh::Vertex& vertex, const Matrix4x4& model, const Matrix3x3& normalMatrix, Vector4& screenPos, std::vector<ShadingInOutFields>& vertexOut) {
	ShadingInOutFields out = ShadingInOutFields();
	out.ClipSpacePosition = projectionMatrix * viewMatrix * model * Vector4(vertex.Position, 1.0);
	if (out.ClipSpacePosition.w > 0) {
		// flip y axis, since the top left corner on windows is the origin, while the bottom left corner on openGL is the origin.
		out.NDC = Vector4(out.ClipSpacePosition.x / out.ClipSpacePosition.w, -out.ClipSpacePosition.y / out.ClipSpacePosition.w, out.ClipSpacePosition.z / out.ClipSpacePosition.w, out.ClipSpacePosition.w / out.ClipSpacePosition.w);
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
		vertexOut.push_back(out);
	}
}

void SoftwareRendering::Rasterize(int startIndex, const std::vector<ShadingInOutFields>& vertexOutFields, std::vector<ShadingInOutFields>& fragmentInFields) {
	const Vector2 pixelSize = Vector2(1.f / Settings::ScreenWidth, 1.f / Settings::ScreenHeight);
	Vector2 screenPos1 = ViewportTransform(vertexOutFields[startIndex].NDC);
	Vector2 screenPos2 = ViewportTransform(vertexOutFields[startIndex + 1].NDC);
	Vector2 screenPos3 = ViewportTransform(vertexOutFields[startIndex + 2].NDC);
	float minX = Mathf::Min({ screenPos1.x, screenPos2.x, screenPos3.x });
	float maxX = Mathf::Max({ screenPos1.x, screenPos2.x, screenPos3.x });
	float minY = Mathf::Min({ screenPos1.y, screenPos2.y, screenPos3.y });
	float maxY = Mathf::Max({ screenPos1.y, screenPos2.y, screenPos3.y });
	// Pixel is always on positions with an integer subscript, so we iterate over integers
	for (int i = Mathf::Max((int)minX, 0); i < (int)Mathf::Min((int)maxX, Settings::ScreenWidth); i++) {
		for (int j = Mathf::Max((int)minY, 0); j < (int)Mathf::Min((int)maxY, Settings::ScreenHeight); j++) {
			if (InTriangle(Vector2(i + pixelSize.x / 2, j + pixelSize.y / 2), screenPos1, screenPos2, screenPos3)) {
				// For all pixels in the triangle, we interpolate the vertex and get one ShadingInOutFields for fragment shading
				float lambda12 = std::abs(Vector2::Cross(screenPos1 - screenPos2, Vector2(i, j) - screenPos2) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda13 = std::abs(Vector2::Cross(screenPos3 - screenPos1, Vector2(i, j) - screenPos1) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda23 = std::abs(Vector2::Cross(screenPos2 - screenPos3, Vector2(i, j) - screenPos3) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				ShadingInOutFields in;
				float z1 = vertexOutFields[startIndex].ClipSpacePosition.z;
				float z2 = vertexOutFields[startIndex + 1].ClipSpacePosition.z;
				float z3 = vertexOutFields[startIndex + 2].ClipSpacePosition.z;
				float z4 = 1 / (lambda12 / z3 + lambda13 / z2 + lambda23 / z1);
				in.ClipSpacePosition = z4 * (lambda12 * vertexOutFields[startIndex + 2].ClipSpacePosition / z3 + lambda13 * vertexOutFields[startIndex + 1].ClipSpacePosition / z2 + lambda23 * vertexOutFields[startIndex].ClipSpacePosition / z1);
				in.NDC = z4 * (lambda12 * vertexOutFields[startIndex + 2].NDC / z3 + lambda13 * vertexOutFields[startIndex + 1].NDC / z2 + lambda23 * vertexOutFields[startIndex].NDC / z1);
				in.FragPos = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPos / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPos / z2 + lambda23 * vertexOutFields[startIndex].FragPos / z1);
				in.FragPosLightSpace = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPosLightSpace / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPosLightSpace / z2 + lambda23 * vertexOutFields[startIndex].FragPosLightSpace / z1);
				in.Normal = Vector3::Normalize(z4 * (lambda12 * vertexOutFields[startIndex + 2].Normal / z3 + lambda13 * vertexOutFields[startIndex + 1].Normal / z2 + lambda23 * vertexOutFields[startIndex].Normal / z1));
				in.TexCoord = z4 * (lambda12 * vertexOutFields[startIndex + 2].TexCoord / z3 + lambda13 * vertexOutFields[startIndex + 1].TexCoord / z2 + lambda23 * vertexOutFields[startIndex].TexCoord / z1);
				in.T = z4 * (lambda12 * vertexOutFields[startIndex + 2].T / z3 + lambda13 * vertexOutFields[startIndex + 1].T / z2 + lambda23 * vertexOutFields[startIndex].T / z1);
				in.N = z4 * (lambda12 * vertexOutFields[startIndex + 2].N / z3 + lambda13 * vertexOutFields[startIndex + 1].N / z2 + lambda23 * vertexOutFields[startIndex].N / z1);
				in.B = z4 * (lambda12 * vertexOutFields[startIndex + 2].B / z3 + lambda13 * vertexOutFields[startIndex + 1].B / z2 + lambda23 * vertexOutFields[startIndex].B / z1);
				in.ScreenPosition = Vector2(i, j);

				fragmentInFields.push_back(in);
			}
		}
	}
}

Vector2 SoftwareRendering::ViewportTransform(const Vector4& ndc) {
	return Vector2(ndc.x * Settings::ScreenWidth / 2 + Settings::ScreenWidth / 2, ndc.y * Settings::ScreenHeight / 2 + Settings::ScreenHeight / 2);
}

bool KritiaEngine::Rendering::SoftwareRendering::InTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c) {
	Vector2 ab = b - a;
	Vector2 bc = c - b;
	Vector2 ca = a - c;
	if (Vector2::Cross(bc, p - b) * Vector2::Cross(bc, a - b) > 0 && Vector2::Cross(ca, p - c) * Vector2::Cross(ca, b - c) > 0 && Vector2::Cross(ab, p - a) * Vector2::Cross(ab, c - a) > 0) {
		return true;
	} else {
		return false;
	}
}
void SoftwareRendering::FragmentShading(const std::shared_ptr<Material>& material, const std::vector<ShadingInOutFields>& inFields, const Vector3& viewPos, const Vector3& pos) {
	int x = 0;
	if (material->renderMode == Material::RenderMode::Opaque) {
        //#pragma omp parallel for
		for (int i = 0; i < inFields.size(); i++) {
			ShadingInOutFields in = inFields[i];
			// Early-Z
			if (in.NDC.z > depthBuffer[in.ScreenPosition.x][in.ScreenPosition.y]) {
				continue;
			}
			float previousZ = depthBuffer[in.ScreenPosition.x][in.ScreenPosition.y];
			depthBuffer[in.ScreenPosition.x][in.ScreenPosition.y] = in.NDC.z;
			Matrix3x3 TBN = Matrix3x3(Vector3::Normalize(in.T), Vector3::Normalize(in.B), Vector3::Normalize(in.N));
			Vector3 viewDir = Vector3::Normalize(viewPos - in.FragPos);
			Light* mainLight = LightingSystem::GetMainLightSource();
			Vector3 lightDir = -Vector3::Normalize(mainLight->Transform()->forward);

			Vector3 norm;
			if (material->normalMap != nullptr) {
				norm = SampleTexture(material->normalMap, in.TexCoord).GetRGB();
				norm = Vector3::Normalize(norm * 2 - Vector3(1, 1, 1));
				norm - Vector3::Normalize(TBN * norm);
			} else {
				norm = Vector3::Normalize(in.Normal);
			}
			// back face culling
			if (Vector3::Dot(-viewDir, norm) > 0) {
				continue;
			}
			Vector2 texCoord;
			if (material->parallaxMap != nullptr) {
				texCoord = ComputeParallaxMapping(material, in.TexCoord, viewDir);
				if (texCoord.x > 1.f || texCoord.y > 1.f || texCoord.x < 0.f || texCoord.y < 0.f) {
					// Discard the fragment
					depthBuffer[in.ScreenPosition.x][in.ScreenPosition.y] = previousZ;
					continue; 
				}
			} else {
				texCoord = in.TexCoord;
			}
			Vector3 ambientComp = mainLight->ambientIntensity * mainLight->color.GetRGB() * material->albedo.GetRGB() * SampleTexture(material->mainTexture, texCoord).GetRGB();
			float diffuseFactor = Mathf::Max(Vector3::Dot(norm, lightDir), 0.f);
			Vector3 diffuseComp = diffuseFactor * mainLight->diffuseIntensity * mainLight->color.GetRGB() * material->albedo.GetRGB() * SampleTexture(material->mainTexture, texCoord).GetRGB();

			Vector3 halfwayDir = Vector3::Normalize(lightDir + viewDir);
			float specularFactor = std::pow(Mathf::Max(Vector3::Dot(norm, halfwayDir), 0.f), material->shininess);
			Vector3 specularComp = specularFactor * mainLight->specularIntensity * mainLight->color.GetRGB() * SampleTexture(material->specularMap, texCoord).GetRGB();
			float shadow = ComputeMainShadow(material, in.FragPosLightSpace);
			Color finalColor = Color((ambientComp + (1 - shadow) * (diffuseComp + specularComp)), 1.f);
			if (shadow < 1 && diffuseFactor != 0) {
				x++;
			}
			//std::vector<Light*> pointLights = LightingSystem::GetPointLightAroundPos(pos);
			//std::vector<Light*> spotLights = LightingSystem::GetSpotLightAroundPos(pos);
			//for (int j = 0; j < Mathf::Min((int)pointLights.size(), LightingSystem::MaxPointLightsForOneObject); j++) {
			//	finalColor += ComputePointLight(material, pointLights[j], norm, in.FragPos, viewDir, texCoord, material->albedo.GetRGB(), in, viewPos);
			//}
			//for (int j = 0; j < Mathf::Min((int)spotLights.size(), LightingSystem::MaxSpotLightsForOneObject); j++) {
			//	finalColor += ComputeSpotLight(material, spotLights[j], norm, in.FragPos, viewDir, texCoord, material->albedo.GetRGB(), in);
			//}
			//std::cout << ambientComp.x << " " << ambientComp.y << " " << ambientComp.z << std::endl;
			//frameBuffer[(int)in.ScreenPosition.x][(int)in.ScreenPosition.y] = Color(norm, 1.f);
			frameBuffer[(int)in.ScreenPosition.x][(int)in.ScreenPosition.y] = finalColor;
		}
	}
	int y = x;
}

Vector2 SoftwareRendering::ComputeParallaxMapping(const std::shared_ptr<Material>& material, const Vector2& texCoord, const Vector3& viewDir) {
	const int depthLayers = 10;
	const float heightScale = 0.1f;
	// calculate the size of each layer
	float layerDepth = 1.0 / depthLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// the amount to shift the texture coordinates per layer (from vector P)
	Vector2 P = heightScale * Vector2(viewDir.x, viewDir.y) ;
	Vector2 deltaTexCoords = P / depthLayers;
	// get initial values
	Vector2 currentTexCoords = texCoord;
	float currentDepthMapValue = SampleTexture(material->parallaxMap, currentTexCoords).r;

	while (currentLayerDepth < currentDepthMapValue) {
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = SampleTexture(material->parallaxMap, currentTexCoords).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}

	// get texture coordinates before collision (reverse operations)
	Vector2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = SampleTexture(material->parallaxMap, prevTexCoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	Vector2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;
}

Color SoftwareRendering::ComputePointLight(const std::shared_ptr<Material>& material, Light* pointLight, const Vector3& normal, const Vector3& fragPos, const Vector3& viewDir, const Vector2& texCoord, const Vector3& albedo, const ShadingInOutFields& inField, const Vector3& viewPos) {
	Vector3 pointLightDir = Vector3::Normalize(pointLight->Transform()->position - fragPos);
	float diff = Mathf::Max(Vector3::Dot(normal, pointLightDir), 0.f);
	Vector3 halfwayDir = Vector3::Normalize(pointLightDir + viewDir);
	float spec = std::pow(Mathf::Max(Vector3::Dot(normal, halfwayDir), 0.f), material->shininess);
	float distance = Vector3::Magnitude(pointLight->Transform()->position - fragPos);
	float attenuation = 1.0 / (pointLight->constantAttenuationFactor + pointLight->linearAttenuationFactor * distance + pointLight->quadraticAttenuationFactor * (distance * distance));
	Vector3 ambient = pointLight->ambientIntensity * pointLight->color.GetRGB() * SampleTexture(material->mainTexture, texCoord).GetRGB() * albedo;
	Vector3 diffuse = pointLight->diffuseIntensity * pointLight->color.GetRGB() * diff * SampleTexture(material->mainTexture, texCoord).GetRGB() * albedo;
	Vector3 specular = pointLight->specularIntensity * pointLight->color.GetRGB() * spec * SampleTexture(material->specularMap, texCoord).GetRGB();
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	float shadow = ComputePointShadow(material, pointLight, inField, viewPos);
	return Color(ambient + (1 - shadow) * (diffuse + specular), 1.0);
}
Color SoftwareRendering::ComputeSpotLight(const std::shared_ptr<Material>& material, Light* spotLight, const Vector3& normal, const Vector3& fragPos, const Vector3& viewDir, const Vector2& texCoord, const Vector3& albedo, const ShadingInOutFields& inField) {
	Vector3 lightDir = Vector3::Normalize(spotLight->Transform()->position - fragPos);
	float theta = Vector3::Dot(lightDir, Vector3::Normalize(-spotLight->Transform()->forward));
	float epsilon = Mathf::Cos(spotLight->cutOffAngleInner) - Mathf::Cos(spotLight->cutOffAngleOuter);
	float intensity = std::clamp((theta - Mathf::Cos(spotLight->cutOffAngleOuter)) / epsilon, 0.f, 1.f);
	float distance = Vector3::Magnitude(spotLight->Transform()->position - fragPos);
	float attenuation = 1.0 / (spotLight->constantAttenuationFactor + spotLight->linearAttenuationFactor * distance + spotLight->quadraticAttenuationFactor * (distance * distance));

	float diff = Mathf::Max(Vector3::Dot(normal, lightDir), 0.f);
	Vector3 halfwayDir = Vector3::Normalize(lightDir + viewDir);
	float spec = std::pow(Mathf::Max(Vector3::Dot(normal, halfwayDir), 0.f), material->shininess);

	Vector3 ambient = spotLight->ambientIntensity * spotLight->color.GetRGB() * SampleTexture(material->mainTexture, texCoord).GetRGB() * albedo;
	Vector3 diffuse = spotLight->diffuseIntensity * spotLight->color.GetRGB() * diff * SampleTexture(material->mainTexture, texCoord).GetRGB() * albedo;
	Vector3 specular = spotLight->specularIntensity * spotLight->color.GetRGB() * spec * SampleTexture(material->specularMap, texCoord).GetRGB();

	ambient *= intensity * attenuation;
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;
	float shadow = ComputeSpotShadow(material, spotLight, inField);
	return Color(ambient + (1 - shadow) * (diffuse + specular), 1.f);
}

float KritiaEngine::Rendering::SoftwareRendering::ComputeMainShadow(const std::shared_ptr<Material>& material, const Vector4& fragPosLightSpace) {
	Light* mainLight = LightingSystem::GetMainLightSource();
	Vector3 projCoords = fragPosLightSpace / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + Vector3(0.5, 0.5, 0.5);
	float shadow = 0;
	if (projCoords.z > 1) {
		shadow = 1.0;
	} else {
		float closestDepth = SampleShadowMap(mainLight, Vector2(projCoords.x, projCoords.y));
		float currentDepth = projCoords.z;
		Vector2 texelSize = 1.0 / Vector2(Settings::ShadowWidth, Settings::ShadowHeight);
		for (int x = -1; x <= 1; ++x) {
			for (int y = -1; y <= 1; ++y) {
				float pcfDepth = SampleShadowMap(mainLight, Vector2(projCoords.x, projCoords.y) + texelSize * Vector2(x, y));
				// 0.005 for bias
				shadow += currentDepth - 0.005 > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /= 9.0;
	}
	return shadow;
}

float SoftwareRendering::ComputePointShadow(const std::shared_ptr<Material>& material, Light* pointLight, const ShadingInOutFields& inField, const Vector3& viewPos) {

	float shadow = 0.0;
	//Vector3 fragToLight = inField.FragPos - pointLight->Transform()->position;
	//float currentDepth = Vector3::Magnitude(fragToLight);
	//float viewDistance = Vector3::Magnitude(viewPos - inField.FragPos);
	//float diskRadius = (1.0 + (viewDistance / Settings::FarPlaneDistance)) / 25.0;
	//for (int i = 0; i < pointLightPcfSamples; ++i) {
	//	float closestDepth = SampleCubeTexture(shadowMapPoint[pointLight->shadowMapPointID], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
	//	closestDepth *= Settings::FarPlaneDistance;   // Undo mapping [0;1]
	//	// may need bias
	//	shadow += currentDepth > closestDepth ? 1.0 : 0.0;
	//}
	//shadow /= float(pointLightPcfSamples);
	return shadow;
}

float SoftwareRendering::ComputeSpotShadow(const std::shared_ptr<Material>& material, Light* spotLight, const ShadingInOutFields& inField) {
	Vector4 fragPosLightSpace = spotLight->GetLightMatrixVP(0) * Vector4(inField.FragPos, 1.0);
	Vector3 projCoords = Vector3(fragPosLightSpace) / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + Vector3(0.5, 0.5, 0.5);
	float shadow = 0;
	//if (projCoords.z > 1.0) {
	//	shadow = 1.0;
	//} else {
	//	float closestDepth = SampleTexture(shadowMap[spotLight->shadowMapID], Vector2(projCoords.x, projCoords.y)).r;
	//	float currentDepth = projCoords.z;
	//	Vector2 texelSize = 1.0 / shadowMap[LightingSystem::GetMainLightSource()->shadowMapID]->size;
	//	for (int x = -1; x <= 1; ++x) {
	//		for (int y = -1; y <= 1; ++y) {
	//			float pcfDepth = SampleTexture(shadowMap[spotLight->shadowMapID], Vector2(projCoords.x, projCoords.y) + texelSize * Vector2(x, y)).r;
	//			// perhaps need bias
	//			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
	//		}
	//	}
	//	shadow /= 9.0;
	//}
	return shadow;
}


float SoftwareRendering::SampleShadowMap(Light* light, Vector2 texCoord) {
	if (texCoord.x > 1) {
		texCoord.x -= (int)texCoord.x;
	}
	if (texCoord.y > 1) {
		texCoord.y -= (int)texCoord.y;
	}
	return shadowFramebuffers[light->shadowMapFBO][int(texCoord.x * Settings::ShadowWidth)][int(texCoord.y * Settings::ShadowHeight)];
}


Color KritiaEngine::Rendering::SoftwareRendering::SampleTexture(const std::shared_ptr<Texture>& texture, Vector2 texCoord) {
	if (texCoord.x > 1) {
		texCoord.x -= (int)texCoord.x;
	}
	if (texCoord.y > 1) {
		texCoord.y -= (int)texCoord.y;
	}
	unsigned char* pixelOffset = textures[texture->ID] + ((unsigned int)(texCoord.x * texture->size.x) + (unsigned int)(texCoord.y * texture->size.y) * (unsigned int)texture->size.x) * texture->channels;
	float offset = ((unsigned int)(texCoord.x * texture->size.x) + (unsigned int)(texCoord.y * texture->size.y) * (unsigned int)texture->size.x) * texture->channels;
	if (texture->channels == 3) {
		return Color((int)pixelOffset[0] / 255.f, (int)pixelOffset[1] / 255.f, (int)pixelOffset[2] / 255.f, 1.f);
	} else {
		return Color((int)pixelOffset[0] / 255.f, (int)pixelOffset[1] / 255.f, (int)pixelOffset[2] / 255.f, (int)pixelOffset[3] / 255.f);
	}
}

Color SoftwareRendering::SampleCubeTexture(const std::vector<std::shared_ptr<Texture>>& textures, const Vector3& direction) {
	return Color();
}

void SoftwareRendering::DrawPixel(const Vector2& position, const Color& color) {
	SetPixel(dc, (int)position.x, (int)position.y, RGB(color.r * 255, color.g * 255, color.b * 255));
	
}
