#include "RenderManager.h"
#include "../CoreModule/Settings.h"
#include <stb_image.h>
#include "../CoreModule/Lighting.h"
#include "../CoreModule/Mathf.h"
#include "../Component/Transform.h"

bool KritiaEngine::RenderManager::depthTestEnabled = true;
bool KritiaEngine::RenderManager::blendEnabled = true;
bool KritiaEngine::RenderManager::backFaceCullingEnabled = true;
unsigned int KritiaEngine::RenderManager::skyboxVAO = 1;
unsigned int KritiaEngine::RenderManager::skyboxVBO = 1;
unsigned int KritiaEngine::RenderManager::uniformBufferIDMatricesVP = 0;

void KritiaEngine::RenderManager::Initialize() {
	if (Settings::UseOpenGL) {
		OpenGLInitialize();
		CreateUniformBuffer(&uniformBufferIDMatricesVP, MatricesVP);
	}
	CreateSkybox();
}

void KritiaEngine::RenderManager::ClearFramebuffer() {
	if (Settings::UseOpenGL) {
		OpenGLClearFramebuffer();
	}
}

void KritiaEngine::RenderManager::LoadCubeMap(std::vector<std::shared_ptr<Texture>> skyboxTextures, unsigned int* id) {
	int width, height, nrChannels;
	if (Settings::UseOpenGL) {
		glGenTextures(1, id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, *id);
		for (unsigned int i = 0; i < skyboxTextures.size(); i++) {
			unsigned char* data = stbi_load(skyboxTextures[i]->path.c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			} else {
				std::cout << "Cubemap texture failed to load at path: " << skyboxTextures[i]->path.c_str() << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void KritiaEngine::RenderManager::CreateSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &RenderManager::skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

unsigned int KritiaEngine::RenderManager::Load2DTexture(std::shared_ptr<Texture> texture, bool alphaChannel) {
	unsigned int id;
	int width, height, nrChannels;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = nullptr;
	if (alphaChannel) {
		data = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 4);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			{
				std::cout << "Failed to load texture" << std::endl;
			}
		}

	} else {
		data = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 3);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			{
				std::cout << "Failed to load texture" << std::endl;
			}
		}
	}
	stbi_image_free(data);
	return id;
}

void KritiaEngine::RenderManager::RenderSkybox(unsigned int skyboxTexutureID, Matrix4x4 projection, Matrix4x4 view, std::shared_ptr<Shader> skyboxShader) {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	// drop translation part
	view = Matrix4x4((Matrix3x3)view);
	skyboxShader->Use();
	skyboxShader->SetInt("skybox", 0);
	skyboxShader->SetMat4("view", view);
	skyboxShader->SetMat4("projection", projection);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexutureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void KritiaEngine::RenderManager::ApplyMaterialShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos,
	                                                          std::shared_ptr<Shader> shader, unsigned int mainTextureID, unsigned int specularMapID) {
	if (Settings::UseOpenGL) {
		shader->Use();
		// main light source should always be a directional light, and there should always be one such light source.
		SetMainLightProperties(shader);
		SetPointLightProperties(pos, shader);
		SetSpotLightProperties(pos, shader);
		// model matrix
		shader->SetMat4("model", model);
		Matrix4x4 mat4 = model;
		Matrix3x3 normalMatrix = Matrix3x3({ mat4.GetEntry(0, 0), mat4.GetEntry(1, 0), mat4.GetEntry(2, 0),
											 mat4.GetEntry(0, 1), mat4.GetEntry(1, 1), mat4.GetEntry(2, 1),
											 mat4.GetEntry(0, 2), mat4.GetEntry(1, 2), mat4.GetEntry(2, 2) }).Inverse().Transpose();
		shader->SetMat3("normalMatrix", normalMatrix);
		shader->SetVec3("viewPos", viewPos);

		// bind maps
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMapID);
	}
}

void KritiaEngine::RenderManager::CreateUniformBuffer(unsigned int* id, UniformBindingPoint bindingPoint) {
	if (bindingPoint == MatricesVP) {
		glGenBuffers(1, id);
		glBindBuffer(GL_UNIFORM_BUFFER, *id);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, MatricesVP, *id, 0, 2 * sizeof(glm::mat4));
	}
}

void KritiaEngine::RenderManager::SetupMesh(std::shared_ptr<Mesh> mesh) {
	if (Settings::UseOpenGL) {
		mesh->isSetup = true;
		mesh->submeshSize = mesh->submeshIndices.size();
		mesh->VAOs.resize(mesh->submeshSize);
		mesh->VBOs.resize(mesh->submeshSize);
		mesh->EBOs.resize(mesh->submeshSize);
		for (int i = 0; i < mesh->submeshSize; i++) {
			glGenVertexArrays(1, &mesh->VAOs[i]);
			glGenBuffers(1, &mesh->VBOs[i]);
			glGenBuffers(1, &mesh->EBOs[i]);

			glBindVertexArray(mesh->VAOs[i]);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOs[i]);

			glBufferData(GL_ARRAY_BUFFER, mesh->submeshVertices[i].size() * sizeof(Mesh::Vertex), &mesh->submeshVertices[i][0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBOs[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->submeshIndices[i].size() * sizeof(unsigned int), &mesh->submeshIndices[i][0], GL_STATIC_DRAW);

			// 顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
			// 顶点法线
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
			// 顶点纹理坐标
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoord));

			glBindVertexArray(0);
		}
	}
}

void KritiaEngine::RenderManager::RenderSubmesh(std::shared_ptr<Mesh> mesh, int submeshIndex) {
	glBindVertexArray(mesh->VAOs[submeshIndex]);
	glDrawElements(GL_TRIANGLES, mesh->submeshIndices[submeshIndex].size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void KritiaEngine::RenderManager::UpdateUniformBufferMatricesVP(Matrix4x4 view, Matrix4x4 projection) {
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferIDMatricesVP);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr((glm::mat4)view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr((glm::mat4)projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void KritiaEngine::RenderManager::OpenGLInitialize() {
	if (depthTestEnabled) {
		glEnable(GL_DEPTH_TEST);
	}
	if (blendEnabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (backFaceCullingEnabled) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	glViewport(0, 0, Settings::ScreenWidth, Settings::ScreenHeight);
}

void KritiaEngine::RenderManager::OpenGLClearFramebuffer() {
	//清除颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void KritiaEngine::RenderManager::SetMainLightProperties(std::shared_ptr<Shader> shader) {
	// main light source should always be a directional light, and there should always be one such light source.
	shader->SetVec3("mainLightDirection", Lighting::LightingSystem::MainLightSource->direction);
	shader->SetFloat("ambientIntensity", Lighting::LightingSystem::MainLightSource->ambientIntensity);
	shader->SetFloat("diffuseIntensity", Lighting::LightingSystem::MainLightSource->diffuseIntensity);
	shader->SetFloat("specularIntensity", Lighting::LightingSystem::MainLightSource->specularIntensity);
	shader->SetVec3("mainLightColor", Lighting::LightingSystem::MainLightSource->color.RGB());

}

void KritiaEngine::RenderManager::SetPointLightProperties(const Vector3& pos, std::shared_ptr<Shader> shader) {
	std::vector<std::shared_ptr<Light>> pointLights = Lighting::LightingSystem::GetPointLightAroundPos(pos);
	for (int i = 0; i < Lighting::LightingSystem::MaxPointLightsForOneObject; i++) {
		std::string str = "pointLights[";
		str += std::to_string(i);
		str += "]";
		if (pointLights[i] != nullptr) {
			shader->SetVec3(str + ".color", pointLights[i]->color.RGB());
			shader->SetVec3(str + ".position", pointLights[i]->Transform()->Position);
			shader->SetFloat(str + ".constant", pointLights[i]->constantAttenuationFactor);
			shader->SetFloat(str + ".linear", pointLights[i]->linearAttenuationFactor);
			shader->SetFloat(str + ".quadratic", pointLights[i]->quadraticAttenuationFactor);
			shader->SetFloat(str + ".ambient", pointLights[i]->ambientIntensity);
			shader->SetFloat(str + ".diffuse", pointLights[i]->diffuseIntensity);
			shader->SetFloat(str + ".specular", pointLights[i]->specularIntensity);
		} else {
			break;
		}
	}
}

void KritiaEngine::RenderManager::SetSpotLightProperties(const Vector3& pos, std::shared_ptr<Shader> shader) {
	std::vector<std::shared_ptr<Light>> spotLights = Lighting::LightingSystem::GetSpotLightAroundPos(pos);
	for (int i = 0; i < Lighting::LightingSystem::MaxSpotLightsForOneObject; i++) {
		std::string str = "spotLights[";
		str += std::to_string(i);
		str += "]";
		if (spotLights[i] != nullptr) {
			shader->SetVec3(str + ".color", spotLights[i]->color.RGB());
			shader->SetVec3(str + ".position", spotLights[i]->Transform()->Position);
			shader->SetVec3(str + ".direction", spotLights[i]->direction);
			shader->SetFloat(str + ".constant", spotLights[i]->constantAttenuationFactor);
			shader->SetFloat(str + ".linear", spotLights[i]->linearAttenuationFactor);
			shader->SetFloat(str + ".quadratic", spotLights[i]->quadraticAttenuationFactor);
			shader->SetFloat(str + ".cutOffCosInner", Mathf::Cos(spotLights[i]->cutOffAngleInner));
			shader->SetFloat(str + ".cutOffCosOuter", Mathf::Cos(spotLights[i]->cutOffAngleOuter));
			shader->SetFloat(str + ".ambient", spotLights[i]->ambientIntensity);
			shader->SetFloat(str + ".diffuse", spotLights[i]->diffuseIntensity);
			shader->SetFloat(str + ".specular", spotLights[i]->specularIntensity);
		} else {
			break;
		}
	}
}

float KritiaEngine::RenderManager::skyboxVertices[108] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

