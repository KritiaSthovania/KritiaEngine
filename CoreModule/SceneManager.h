#pragma once
#include "Scene.h"
namespace KritiaEngine::SceneManagement{

	static class SceneManager
	{
	public:
		static void Initialize(bool inEditor);
		// ����������һ������
		static std::shared_ptr<Scene> CreateScene(const std::string &name);
		static void LoadScene(std::shared_ptr<Scene> scene);
		static void Render();
		static void Update();
		static std::shared_ptr<Scene> GetActiveScene();
	private:
		static std::shared_ptr<Scene> activeScene;
		// if we are in editor or in game
		static bool inEditor;
	};

}


