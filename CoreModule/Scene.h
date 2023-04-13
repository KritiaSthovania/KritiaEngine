#pragma once

#include "GameObject.h"
#include "Texture.h"
#include "Interface/ISerializable.h"
#include "../Component/Camera.h"
#include "../Rendering/Shader.h"
#include "../Editor/EditorApplication.h"
#include <json/json.hpp>

//#define Generate

namespace KritiaEngine::SceneManagement {
	class Scene : private IFileSerializable
	{
		friend class SceneManager;
	public:
		~Scene();
		std::string name;
		/// <summary>
		/// Returns a vector of all root game objects in the scene.
		/// </summary>
		std::list<GameObject*>& GetRootGameObjects();
		/// <summary>
		/// The full path (with the file name of the scene)
		/// </summary>
		std::string path;
	private:
		Scene(const std::string& name, const std::string& path = "");
		// All initialize functions don't load resources for now.
		void Initialize();
		void InitializeCamera();
		void InitializeLighting();
		void InitializeGameObjects();
		/// <summary>
		/// Save the serialized json data to file.
		/// </summary>
		virtual void SerializeToFile() override;
		virtual void DeserializeFromPath(const std::string& path);
		/// <summary>
		/// current rendering camera
		/// </summary>
		GameObject* currentCamera;
		std::list<GameObject*> rootGameObjects;
		GameObject* mainLightSource;
		std::list<GameObject*> opaqueObjects;
		std::list<GameObject*> transparentObjects;

	};

}

