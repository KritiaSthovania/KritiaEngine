#include "RendererManager.h"
#include "../../Rendering/RenderingProvider.h"
#include "../../CoreModule/Settings.h"
#include "../../Component/Renderer/MeshRenderer.h"

using namespace KritiaEngine::Manager;
using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;
std::list<Renderer*> RendererManager::opaqueRenderQueue = std::list<Renderer*>();
std::list<Renderer*> RendererManager::transparentRenderQueue = std::list<Renderer*>();


void KritiaEngine::Manager::RendererManager::AddMeshRenderer(MeshRenderer* renderer, bool transparent) {
	if (transparent) {
		transparentRenderQueue.push_back(renderer);
	} else {
		opaqueRenderQueue.push_back(renderer);
	}
}

void KritiaEngine::Manager::RendererManager::RemoveMeshRenderer(MeshRenderer* renderer, bool transparent) {
	if (transparent) {
		transparentRenderQueue.remove(renderer);
	} else {
		opaqueRenderQueue.remove(renderer);
	}
}

void KritiaEngine::Manager::RendererManager::MoveMeshRendererToTransparentQueue(MeshRenderer* renderer) {
	opaqueRenderQueue.remove(renderer);
	transparentRenderQueue.push_back(renderer);
}

void KritiaEngine::Manager::RendererManager::Render() {
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Zoom, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistant);
	RenderingProvider::UpdateUniformBufferMatricesVP(Camera::current->GetViewMatrix(), projection);
	opaqueRenderQueue.sort(CompareRenderer);
	// Render shadow map only for opaque objects
	RenderingProvider::SetupRenderShadowMap();
	for (auto renderer : opaqueRenderQueue) {
		renderer->RenderShadowMap(Lighting::LightingSystem::MainLightSource);
	}
	// Render opaque objects from near to far
	RenderingProvider::SetupRenderSubmesh();
	for (auto renderer : opaqueRenderQueue) {
		renderer->Render(Camera::current);
	}
	RenderingProvider::RenderGPUInstances(false);
	RenderSkybox();
	transparentRenderQueue.sort(CompareRenderer);
	// Render transparent objects from far to near
	for (auto iter = transparentRenderQueue.rbegin(); iter != transparentRenderQueue.rend(); iter++) {
		(*iter)->Render(Camera::current);
	}
	RenderingProvider::RenderGPUInstances(true);
}

bool KritiaEngine::Manager::RendererManager::CompareRenderer(Renderer * left, Renderer * right) {
	return Vector3::Magnitude(left->Transform()->Position - Camera::current->GetPosition()) < Vector3::Magnitude(right->Transform()->Position - Camera::current->GetPosition());
}

void KritiaEngine::Manager::RendererManager::RenderSkybox() {
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Zoom, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistant);
	Matrix4x4 view = Camera::current->GetViewMatrix();
	RenderingProvider::RenderSkybox(projection, view);
}

