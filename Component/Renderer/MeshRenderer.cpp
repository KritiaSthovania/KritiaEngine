#include "MeshRenderer.h"
#include "../../CoreModule/MathStructs.h"
#include "../../CoreModule/GameObject.h"
#include "../../Rendering/RenderingProvider.h"
#include "../../CoreModule/Manager/RendererManager.h"
#include "../../Editor/EditorApplication.h"

using namespace KritiaEngine;
using namespace KritiaEngine::Manager;
using namespace KritiaEngine::Rendering;
using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

MeshRenderer::MeshRenderer(GameObject* gameObject) {
	this->gameObject = gameObject;
	PreInitializeMaterial();
	RendererManager::AddMeshRenderer(this, containTransparentMaterial);
}

void KritiaEngine::MeshRenderer::PreInitializeMaterial() {
	if (this->gameObject->GetComponent<MeshFilter>() != nullptr && this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size() > 0) {
		for (int i = 0; i < this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(); i++) {
			materials.push_back(this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials[i]);
			if (this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials[i]->renderMode == Material::Transparent) {
				containTransparentMaterial = true;
			}
		}
		materialSize = materials.size();
		preInitialized = true;
	}
}

void MeshRenderer::InitializeMaterial() {
	// At the moment the renderer was added, the mesh has not been added, so we must add the materials of the mesh first.
	if (!preInitialized) {
		if (this->gameObject->GetComponent<MeshFilter>() != nullptr && this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size() > 0) {
			for (int i = 0; i < this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(); i++) {
				materials.push_back(this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials[i]);
				if (this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials[i]->renderMode == Material::Transparent) {
					// Move the renderer to the transparent queue
					if (!containTransparentMaterial) {
						RendererManager::MoveMeshRendererToTransparentQueue(this);
						containTransparentMaterial = true;
					}
				}
			}
			materialSize = materials.size();
			preInitialized = true;
		}
	}
	for (int i = 0; i < materials.size(); i++) {
		materials[i]->Initialize();
		if (materials[i]->renderMode == Material::Transparent) {
			if (!containTransparentMaterial) {
				// Move the renderer to the transparent queue
				RendererManager::MoveMeshRendererToTransparentQueue(this);
				containTransparentMaterial = true;
			}
		}
	}
}

void KritiaEngine::MeshRenderer::UpdateMaterial() {
	if (materialSize > materials.size()) {
		materialSize = materials.size();
	} else {
		for (int i = materialSize; i < materials.size(); i++) {
			materials[i]->Initialize();
			if (materials[i]->renderMode == Material::Transparent) {
				if (!containTransparentMaterial) {
					RendererManager::MoveMeshRendererToTransparentQueue(this);
					containTransparentMaterial = true;
				}
			}
		}
		materialSize = materials.size();
	}
}

void KritiaEngine::MeshRenderer::OnInspector() {
	ImGui::Text("Materials");
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("##Materials", ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (int i = 0; i < materials.size(); i++) {
			//TODO: Drag And Drop
			if (ImGui::Button(materials[i]->name.c_str())) {
				const char* path = ImguiAlias::OpenFindResourceWindow("Material", materialFilePostfix);
				materials[i] = Material::DeserializeFromPath(path);
			}
		}
	}
}

std::string KritiaEngine::MeshRenderer::Serialize() {
	json json;
	json["Type"] = "MeshRenderer";
	return json.dump();
}

void KritiaEngine::MeshRenderer::Deserialize(const json& json) {
	assert(json["Type"] == "MeshRenderer");
}

std::string KritiaEngine::MeshRenderer::GetInspectorLabel() {
	return inspectorLabel;
}

void MeshRenderer::Initialize() {
	meshFilter = gameObject->GetComponent<MeshFilter>();
	meshFilter->SetupMesh();
	InitializeMaterial();
	initialized = true;
}

void KritiaEngine::MeshRenderer::OnObjectDestroy() {
	RendererManager::RemoveMeshRenderer(this, containTransparentMaterial);
}

void MeshRenderer::Render(const std::shared_ptr<KritiaEngine::Camera>& camera) {
	if (!initialized) {
		Initialize();
	}
	// materials have changed
	if (materialSize != materials.size()) {
		UpdateMaterial();
	}
	Matrix4x4 model = Matrix4x4::Identity();
	model = Mathf::Translate(model, Transform()->position);
	model *= Quaternion::ToRotationMatrix(Transform()->rotation);
	model = Mathf::Scale(model, Transform()->scale);
	for (int i = 0; i < meshFilter->mesh->submeshSize; i++) {
		RenderingProvider::RenderSubmesh(meshFilter, materials[i], i, model, camera->GetPosition(), Transform()->position);
	}
	// 如果材质数大于Submesh数
	for (int i = this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(); i < materials.size(); i++) {
		RenderingProvider::RenderSubmesh(meshFilter, materials[i], i - this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(), model, camera->GetPosition(), Transform()->position);
	}
}


void KritiaEngine::MeshRenderer::RenderShadowMap(Light* light) {
	if (light->castingShadow) {
		if (!initialized) {
			Initialize();
		}
		// materials have changed
		if (materialSize != materials.size()) {
			UpdateMaterial();
		}
		Matrix4x4 model = Matrix4x4::Identity();
		model = Mathf::Translate(model, Transform()->position);
		model *= Quaternion::ToRotationMatrix(Transform()->rotation);
		model = Mathf::Scale(model, Transform()->scale);
		for (int i = 0; i < meshFilter->mesh->submeshSize; i++) {
			RenderingProvider::RenderShadowMap(meshFilter, i, model, light);
		}
	}
}


