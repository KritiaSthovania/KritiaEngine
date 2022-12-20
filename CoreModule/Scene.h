#pragma once

#include "../Component/Camera.h"
#include "GameObject.h"
#include "Texture.h"
#include "../Rendering/Shader.h"

namespace KritiaEngine::SceneManagement {
	class Scene
	{
		friend class SceneManager;
	public:
		std::string name;
		/// <summary>
		/// Returns a vector of all root game objects in the scene.
		/// </summary>
		std::list<std::shared_ptr<GameObject>> GetRootGameObjects();
		std::vector<std::shared_ptr<Texture>> skyboxTextures;
	private:
		Scene(const std::string &name);
		// All initialize functions don't load resources for now.
		void Initialize();
		void InitializeSkybox();
		void InitializeCamera();
		void InitializeLighting();
		void InitializeGameObjects();
		void InitializeRenderQueue();
		bool CompareTransparentObjectAndCameraPosition(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2);
		/// <summary>
		/// Call all Render() functions of all renderer in the scene
		/// </summary>
		void Render(std::shared_ptr<Camera> camera);
		void RenderOpaque(std::shared_ptr<Camera> camera);
		void RenderTransparent(std::shared_ptr<Camera> camera);
		// The render of skybox is done in RenderManager and not MeshRenderer
		void RenderSkybox(std::shared_ptr<Camera> camera);
		/// <summary>
		/// Call all Update() functions of all MonoBehaviors in the scene
		/// </summary>
		void Update();
		/// <summary>
		/// current rendering camera
		/// </summary>
		std::shared_ptr<GameObject> currentCamera;
		std::list<std::shared_ptr<GameObject>> rootGameObjects;
		std::shared_ptr<GameObject> mainLightSource;
		std::list<std::shared_ptr<GameObject>> opaqueObjects;
		std::list<std::shared_ptr<GameObject>> transparentObjects;
		unsigned int skyBoxTextureID;
		std::shared_ptr<Shader> skyboxShader = std::shared_ptr<Shader>(new Shader("./StandardShader/SkyboxShader.vs", "./StandardShader/SkyboxShader.fs"));
	};

}

