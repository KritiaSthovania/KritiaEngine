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
	private:
		Scene(const std::string &name);
		// All initialize functions don't load resources for now.
		void Initialize();
		void InitializeCamera();
		void InitializeLighting();
		void InitializeGameObjects();
		/// <summary>
		/// current rendering camera
		/// </summary>
		std::shared_ptr<GameObject> currentCamera;
		std::list<std::shared_ptr<GameObject>> rootGameObjects;
		std::shared_ptr<GameObject> mainLightSource;
		std::list<std::shared_ptr<GameObject>> opaqueObjects;
		std::list<std::shared_ptr<GameObject>> transparentObjects;
	};

}

