#pragma once

#include "GameObject.h"
#include "Texture.h"
#include "Interface/SerializableAndDeserializable.h"
#include "../Component/Camera.h"
#include "../Rendering/Shader.h"
#include "../Editor/EditorApplication.h"
#include <json/json.hpp>

namespace KritiaEngine::SceneManagement {
	class Scene : private FileSerializable, PathDeserializable
	{
		friend class SceneManager;
	public:
		std::string name;
		/// <summary>
		/// Returns a vector of all root game objects in the scene.
		/// </summary>
		std::list<std::shared_ptr<GameObject>> GetRootGameObjects();
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
		virtual void DeserializeFromPath(const std::string& path) override;
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

