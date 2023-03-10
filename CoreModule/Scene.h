#pragma once

#include "../Component/Camera.h"
#include "GameObject.h"
#include "Texture.h"
#include "../Rendering/Shader.h"
#include <json/json.hpp>
#include "../Editor/EditorApplication.h"

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
		std::string path;
	private:
		Scene(const std::string& name, const std::string& path = KritiaEngine::Editor::EditorApplication::assetFolderRootPath);
		// All initialize functions don't load resources for now.
		void Initialize();
		void InitializeCamera();
		void InitializeLighting();
		void InitializeGameObjects();
		/// <summary>
		/// Save the serialized json data to file.
		/// </summary>
		void SerializeToFile();
		void DeserializeFromFile(std::ifstream& instream);
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

