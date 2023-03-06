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
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Fovy, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistance);
	RenderingProvider::UpdateUniformBufferMatricesVP(Camera::current->GetViewMatrix(), projection);
	opaqueRenderQueue.sort(CompareRenderer);
	// Render shadow map only for opaque objects
	for (auto light : Lighting::LightingSystem::Lights) {
		if (light->castingShadow) {
			RenderingProvider::SetupRenderShadowMap(light);
			for (auto renderer : opaqueRenderQueue) {
				renderer->RenderShadowMap(light);
			}	
		}
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

void KritiaEngine::Manager::RendererManager::Clear() {
	opaqueRenderQueue.clear();
	transparentRenderQueue.clear();
}

bool KritiaEngine::Manager::RendererManager::CompareRenderer(Renderer * left, Renderer * right) {
	return Vector3::Magnitude(left->Transform()->position - Camera::current->GetPosition()) < Vector3::Magnitude(right->Transform()->position - Camera::current->GetPosition());
}

void KritiaEngine::Manager::RendererManager::RenderSkybox() {
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Fovy, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistance);
	Matrix4x4 view = Camera::current->GetViewMatrix();
	RenderingProvider::RenderSkybox(projection, view);
}

