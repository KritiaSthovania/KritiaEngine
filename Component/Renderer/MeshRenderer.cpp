#include "MeshRenderer.h"
#include "../../CoreModule/MathStructs.h"
#include "../../CoreModule/GameObject.h"
#include "../../CoreModule/SceneManager.h"

using namespace KritiaEngine;

MeshRenderer::MeshRenderer(GameObject* gameObject) {
	this->gameObject = gameObject;
}

void MeshRenderer::InitializeMaterial() {
	if (this->gameObject->GetComponent<MeshFilter>()!= nullptr && this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size() > 0) {
		for (int i = 0; i < this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(); i++) {
			materials.push_back(this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials[i]);
		}
	}
	for (int i = 0; i < materials.size(); i++) {
		materials[i]->Initialize();
	}
}

void MeshRenderer::Initialize() {
	meshFilter = gameObject->GetComponent<MeshFilter>();
	meshFilter->SetupMesh();
	InitializeMaterial();
	initialized = true;
}

void MeshRenderer::Render(std::shared_ptr<KritiaEngine::Camera> camera) {
	if (!initialized) {
		Initialize();
	}
	Matrix4x4 model = Matrix4x4::Identity();
	model = Mathf::Translate(model, Transform()->Position);
	model *= Quaternion::RotationMatrix(Transform()->Rotation);
	model = Mathf::Scale(model, Transform()->Scale);
	Matrix4x4 projection = Matrix4x4::Perspective(camera->Zoom, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistant);
	Matrix4x4 view = camera->GetViewMatrix();
	for (int i = 0; i < meshFilter->mesh->submeshSize; i++) {
		materials[i]->ApplyShaderOnRender(projection, view, model, camera->GetPosition(), Transform()->Position);
		meshFilter->RenderSubmesh(i);
	}
	for (int i = this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size(); i < materials.size(); i++) {
		materials[i]->ApplyShaderOnRender(projection, view, model, camera->GetPosition(), Transform()->Position);
		meshFilter->RenderSubmesh(i - this->gameObject->GetComponent<MeshFilter>()->mesh->submeshMaterials.size());
	}
}

