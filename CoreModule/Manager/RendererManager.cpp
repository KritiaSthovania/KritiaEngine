#include "RendererManager.h"
#include "../../Rendering/RenderingProvider.h"
#include "../../CoreModule/Settings.h"
#include "../../Component/Renderer/MeshRenderer.h"

using namespace KritiaEngine::Manager;
using namespace KritiaEngine;
using namespace KritiaEngine::Rendering;
std::list<std::weak_ptr<Renderer>> RendererManager::opaqueRenderQueue = std::list<std::weak_ptr<Renderer>>();
std::list<std::weak_ptr<Renderer>> RendererManager::transparentRenderQueue = std::list<std::weak_ptr<Renderer>>();


void KritiaEngine::Manager::RendererManager::AddMeshRenderer(const std::weak_ptr<MeshRenderer>& renderer, bool transparent) {
	if (transparent) {
		transparentRenderQueue.push_back(renderer);
	} else {
		opaqueRenderQueue.push_back(renderer);
	}
}

void KritiaEngine::Manager::RendererManager::RemoveMeshRenderer(const std::weak_ptr<MeshRenderer>& renderer, bool transparent) {
	auto comp = [renderer](const std::weak_ptr<Renderer>& p)-> bool {	return renderer.lock() == p.lock(); };
	if (transparent) {	
		transparentRenderQueue.remove_if(comp);
	} else {
		opaqueRenderQueue.remove_if(comp);
	}
}

void KritiaEngine::Manager::RendererManager::MoveMeshRendererToTransparentQueue(const std::weak_ptr<MeshRenderer>& renderer) {
	auto comp = [renderer](const std::weak_ptr<Renderer>& p)-> bool {return renderer.lock() == p.lock(); };
	opaqueRenderQueue.remove_if(comp);
	transparentRenderQueue.push_back(renderer);
}

void KritiaEngine::Manager::RendererManager::Render() {
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Fovy, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistance, Settings::FarPlaneDistance);
	RenderingProvider::UpdateUniformBufferMatricesVP(Camera::current->GetViewMatrix(), projection);
	opaqueRenderQueue.sort(CompareRenderer);
	// Render shadow map only for opaque objects
	for (auto light : Lighting::LightingSystem::Lights) {
		RenderingProvider::SetupRenderShadowMap(light);
		for (auto &renderer : opaqueRenderQueue) {
			if (renderer.lock()->gameObject->isActive) {
				renderer.lock()->RenderShadowMap(light);
			}
		}
	}
	// Render opaque objects from near to far
	RenderingProvider::SetupRenderSubmesh();
	for (auto &renderer : opaqueRenderQueue) {
		if (renderer.lock()->gameObject->isActive) {
			renderer.lock()->Render(Camera::current);
		}
	}
	RenderingProvider::RenderGPUInstances(false);
	RenderSkybox();
	transparentRenderQueue.sort(CompareRenderer);
	// Render transparent objects from far to near
	for (auto iter = transparentRenderQueue.rbegin(); iter != transparentRenderQueue.rend(); iter++) {
		if ((*iter).lock()->gameObject->isActive) {
			(*iter).lock()->Render(Camera::current);
		}
	}
	RenderingProvider::RenderGPUInstances(true);
}

void KritiaEngine::Manager::RendererManager::Clear() {
	opaqueRenderQueue.clear();
	transparentRenderQueue.clear();
}

bool KritiaEngine::Manager::RendererManager::ComparePtr(const std::weak_ptr<Renderer>& left, const std::weak_ptr<Renderer>& right) {
	return left.lock() == right.lock();
}

bool KritiaEngine::Manager::RendererManager::CompareRenderer(const std::weak_ptr<Renderer>& left, const std::weak_ptr<Renderer>& right) {
	return Vector3::Magnitude(left.lock()->Transform()->position - Camera::current->GetPosition()) < Vector3::Magnitude(right.lock()->Transform()->position - Camera::current->GetPosition());
}

void KritiaEngine::Manager::RendererManager::RenderSkybox() {
	Matrix4x4 projection = Matrix4x4::Perspective(Camera::current->Fovy, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistance, Settings::FarPlaneDistance);
	Matrix4x4 view = Camera::current->GetViewMatrix();
	RenderingProvider::RenderSkybox(projection, view);
}

