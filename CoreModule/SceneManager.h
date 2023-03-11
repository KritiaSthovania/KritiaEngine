#pragma once
#include "Scene.h"
namespace KritiaEngine::SceneManagement{

	class SceneManager
	{
	public:
		static void Initialize(bool inEditor);
		// 创建并返回一个场景
		static std::shared_ptr<Scene> CreateScene(const std::string &name);
		static void LoadScene(const std::shared_ptr<Scene>& scene);
		static void LoadScene(const std::string& path);
		static void SaveScene();
		static std::shared_ptr<Scene> GetActiveScene();
	private:
		static std::shared_ptr<Scene> activeScene;
		// if we are in editor or in game
		static bool inEditor;
	};

}


