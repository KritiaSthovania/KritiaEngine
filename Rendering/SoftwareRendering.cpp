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
std::map<unsigned int, std::vector<std::vector<Color>>> SoftwareRendering::shadowFramebuffers;
std::map<unsigned int, std::shared_ptr<Texture>>  SoftwareRendering::shadowMap;
std::map<unsigned int, std::vector<std::shared_ptr<Texture>>> SoftwareRendering::shadowMapPoint;
std::map<unsigned int, unsigned char*> SoftwareRendering::textures;
unsigned int SoftwareRendering::texture2DCounter = 0;

Vector3 SoftwareRendering::sampleOffsetDirections[pointLightPcfSamples] = { Vector3(1, 1, 1), Vector3(1, -1, 1), Vector3(-1, -1, 1), Vector3(-1, 1, 1),
		   Vector3(1, 1, -1), Vector3(1, -1, -1), Vector3(-1, -1, -1), Vector3(-1, 1, -1),
		   Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0), Vector3(-1, 1, 0),
		   Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
		   Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, -1, -1), Vector3(0, 1, -1) };

void SoftwareRendering::Initialize() {
	for (int i = 0; i < Settings::ScreenWidth; i++) {
		frameBuffer.push_back(std::vector<Color>());
		for (int j = 0; j < Settings::ScreenWidth; j++) {
			frameBuffer[i].push_back(Color(0, 0, 0, 0));
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

void KritiaEngine::Rendering::SoftwareRendering::CreateShadowMap(Light* light) {}

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
				texture->channels = nrChannels;
			} else{
				std::cout << "Failed to load texture at " << texture->path << std::endl;			
			}
		} else {
			textures[id] = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 3);
			if (textures[id]) {
				texture->size = Vector2(width, height);
				texture->channels = nrChannels;
			} else {
				std::cout << "Failed to load texture at " << texture->path << std::endl;
			}
		}
		texture->ID = id;
		texture->loaded = true;
	}
}

unsigned int SoftwareRendering::Load2DTexture(const std::string& path, bool alphaChannel, Vector2& size, int& channel) {
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
			channel = nrChannels;
		} else {
			std::cout << "Failed to load texture at " << path << std::endl;
		}
	} else {
		textures[id] = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
		if (textures[id]) {
			size.x = width;
			size.y = height;
			channel = nrChannels;
		} else {
			std::cout << "Failed to load texture at " << path << std::endl;
		}
	}
	return id;
}

void KritiaEngine::Rendering::SoftwareRendering::SetupRenderSubmesh() {
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
		vertexOutFields.push_back(VertexShading(mesh->submeshVertices[submeshIndex][i], model, normalMatrix, screenPos));
	}
	for (int i = 0; i < mesh->submeshIndices[submeshIndex][i] - 2; i++) {
		Rasterize(i, vertexOutFields, fragmentInFields);
	}
	for (int i = 0; i < vertexOutFields.size(); i++) {
		FragmentShading(material, fragmentInFields, viewPos, pos);
	}
	for (int i = 0; i < frameBuffer.size(); i++) {
		for (int j = 0; j < frameBuffer.size(); j++) {
			DrawPixel(Vector2((float)i / Settings::ScreenWidth, (float)j / Settings::ScreenHeight), frameBuffer[i][j]);
		}
	}
}

SoftwareRendering::ShadingInOutFields SoftwareRendering::VertexShading(const Mesh::Vertex& vertex, const Matrix4x4& model, const Matrix3x3& normalMatrix, Vector4& screenPos) {
	ShadingInOutFields out;
	out.WorldPosition = Vector4(projectionMatrix * viewMatrix * model * Vector4(vertex.Position, 1.0));
	out.NDC = Vector4(out.WorldPosition.x / out.WorldPosition.w, out.WorldPosition.y / out.WorldPosition.w, out.WorldPosition.z / out.WorldPosition.w, out.WorldPosition.w / out.WorldPosition.w);
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

void SoftwareRendering::Rasterize(int startIndex, const std::vector<ShadingInOutFields>& vertexOutFields, std::vector<ShadingInOutFields>& fragmentInFields) {
	Vector2 pixelSize = Vector2(1.f / Settings::ScreenWidth, 1.f / Settings::ScreenHeight);
	Vector2 screenPos1 = ViewportTransform(vertexOutFields[startIndex].NDC);
	Vector2 screenPos2 = ViewportTransform(vertexOutFields[startIndex + 1].NDC);
	Vector2 screenPos3 = ViewportTransform(vertexOutFields[startIndex + 2].NDC);
	float minX = Mathf::Min({ screenPos1.x, screenPos2.x, screenPos3.x });
	float maxX = Mathf::Max({ screenPos1.x, screenPos2.x, screenPos3.x });
	float minY = Mathf::Min({ screenPos1.y, screenPos2.y, screenPos3.y });
	float maxY = Mathf::Max({ screenPos1.y, screenPos2.y, screenPos3.y });
	// Pixel is always on positions with an integer subscript, so we iterate over integers
	for (int i = Mathf::Max((int)minX, 0); i < Mathf::Min((int)maxX, Settings::ScreenWidth); i++) {
		for (int j = Mathf::Max((int)minY, 0); j < Mathf::Min((int)maxY, Settings::ScreenHeight); j++) {
			if (InTriangle(Vector2(i + pixelSize.x / 2, j + pixelSize.y / 2), screenPos1, screenPos2, screenPos3)) {
				// For all pixels in the triangle, we interpolate the vertex and get one ShadingInOutFields for fragment shading
				float lambda12 = std::abs(Vector2::Cross(screenPos1 - screenPos2, Vector2(i, j) - screenPos2) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda13 = std::abs(Vector2::Cross(screenPos3 - screenPos1, Vector2(i, j) - screenPos1) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				float lambda23 = std::abs(Vector2::Cross(screenPos2 - screenPos3, Vector2(i, j) - screenPos3) / Vector2::Cross(screenPos1 - screenPos2, screenPos3 - screenPos2));
				ShadingInOutFields in;
				float z1 = vertexOutFields[startIndex].WorldPosition.z;
				float z2 = vertexOutFields[startIndex + 1].WorldPosition.z;
				float z3 = vertexOutFields[startIndex + 2].WorldPosition.z;
				float z4 = 1 / (lambda12 / z3 + lambda13 / z2 + lambda23 / z1);
				in.WorldPosition = z4 * (lambda12 * vertexOutFields[startIndex + 2].WorldPosition / z3 + lambda13 * vertexOutFields[startIndex + 1].WorldPosition / z2 + lambda23 * vertexOutFields[startIndex].WorldPosition / z1);
				in.NDC = z4 * (lambda12 * vertexOutFields[startIndex + 2].NDC / z3 + lambda13 * vertexOutFields[startIndex + 1].NDC / z2 + lambda23 * vertexOutFields[startIndex].NDC / z1);
				in.FragPos = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPos / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPos / z2 + lambda23 * vertexOutFields[startIndex].FragPos / z1);
				in.FragPosLightSpace = z4 * (lambda12 * vertexOutFields[startIndex + 2].FragPosLightSpace / z3 + lambda13 * vertexOutFields[startIndex + 1].FragPosLightSpace / z2 + lambda23 * vertexOutFields[startIndex].FragPosLightSpace / z1);
				in.Normal = z4 * (lambda12 * vertexOutFields[startIndex + 2].Normal / z3 + lambda13 * vertexOutFields[startIndex + 1].Normal / z2 + lambda23 * vertexOutFields[startIndex].Normal / z1);
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

bool SoftwareRendering::InTriangle(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c) {
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
	if (material->renderMode == Material::RenderMode::Opaque) {
        #pragma omp parallel for
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
			Vector3 lightDir = -Vector3::Normalize(mainLight->Transform()->position);
			Vector3 norm;
			if (material->normalMap != nullptr) {
				norm = SampleTexture(material->normalMap, in.TexCoord).GetRGB();
				norm = Vector3::Normalize(norm * 2 - Vector3(1, 1, 1));
				norm - Vector3::Normalize(TBN * norm);
			} else {
				norm = Vector3::Normalize(in.Normal);
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
			/*std::vector<Light*> pointLights = LightingSystem::GetPointLightAroundPos(pos);
			std::vector<Light*> spotLights = LightingSystem::GetSpotLightAroundPos(pos);
			for (int j = 0; j < Mathf::Min((int)pointLights.size(), LightingSystem::MaxPointLightsForOneObject); j++) {
				finalColor += ComputePointLight(material, pointLights[j], norm, in.FragPos, viewDir, texCoord, material->albedo.GetRGB(), in, viewPos);
			}
			for (int j = 0; j < Mathf::Min((int)spotLights.size(), LightingSystem::MaxSpotLightsForOneObject); j++) {
				finalColor += ComputeSpotLight(material, spotLights[j], norm, in.FragPos, viewDir, texCoord, material->albedo.GetRGB(), in);
			}*/
			//std::cout << ambientComp.x << " " << ambientComp.y << " " << ambientComp.z << std::endl;
			frameBuffer[(int)in.ScreenPosition.x][(int)in.ScreenPosition.y] = finalColor;
		}
	}
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

float SoftwareRendering::ComputeMainShadow(const std::shared_ptr<Material>& material, const Vector4& fragPosLightSpace) {
	Light* mainLight = LightingSystem::GetMainLightSource();
	Vector3 projCoords = (Vector3)fragPosLightSpace / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + Vector3(0.5, 0.5, 0.5);
	float shadow = 0;
	//if (projCoords.z > 1.0) {
	//	shadow = 1.0;
	//} else {
	//	float closestDepth = SampleTexture(shadowMap[mainLight->shadowMapID], Vector2(projCoords.x, projCoords.y)).r;
	//	float currentDepth = projCoords.z;
	//	Vector2 texelSize = 1.0 / shadowMap[LightingSystem::GetMainLightSource()->shadowMapID]->size;
	//	for (int x = -1; x <= 1; ++x) {
	//		for (int y = -1; y <= 1; ++y) {
	//			float pcfDepth = SampleTexture(shadowMap[mainLight->shadowMapID], Vector2(projCoords.x, projCoords.y) + texelSize * Vector2(x, y)).r;
	//			// perhaps need bias
	//			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
	//		}
	//	}
	//	shadow /= 9.0;
	//}
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

Color SoftwareRendering::SampleTexture(const std::shared_ptr<Texture>& texture, const Vector2& texCoord) {
	unsigned char* pixelOffset = textures[texture->ID] + ((unsigned int)texCoord.x + (unsigned int)texCoord.y * (unsigned int)texture->size.x) * texture->channels;
	if (texture->channels == 3) {
		return Color(pixelOffset[0], pixelOffset[1], pixelOffset[2], 1.f);
	} else {
		return Color(pixelOffset[0], pixelOffset[1], pixelOffset[2], pixelOffset[3]);
	}

}

Color SoftwareRendering::SampleCubeTexture(const std::vector<std::shared_ptr<Texture>>& textures, const Vector3& direction) {
	return Color();
}

void SoftwareRendering::DrawPixel(const Vector2& position, const Color& color) {
	glBegin(GL_POINT);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(position.x, position.y);
	glEnd();
}
