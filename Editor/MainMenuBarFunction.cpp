#include "MainMenuBarFunction.h"
#include "EditorApplication.h"
#include "../CoreModule/SceneManager.h"
#include <nfd/nfd.h>
#include <stdio.h>

void KritiaEngine::Editor::GUI::MainMenuBarFunction::OpenScene() {
    NFD_Init();
    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[1] = { { "Scene File", "scene.json"}};
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY) {
        SceneManagement::SceneManager::LoadScene(outPath);
        NFD_FreePath(outPath);
    } else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
    } else {
        printf("Error: %s\n", NFD_GetError());
    }
    NFD_Quit();
}

void KritiaEngine::Editor::GUI::MainMenuBarFunction::SaveScene() {
    if (SceneManagement::SceneManager::GetActiveScene()->path == "") {
        // A new scene
        NFD_Init();
        nfdchar_t* outPath;
        nfdfilteritem_t filterItem[1] = { { "Scene File", "scene.json"} };
        std::cout << EditorApplication::assetFolderRootPath.c_str();
        nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, (EditorApplication::currentProjectFolderPath + EditorApplication::assetFolderRootPath).c_str(), SceneManagement::SceneManager::GetActiveScene()->name.c_str());
        if (result == NFD_OKAY) {
            SceneManagement::SceneManager::GetActiveScene()->path = outPath;
            SceneManagement::SceneManager::SaveScene();
            NFD_FreePath(outPath);
        } else if (result == NFD_CANCEL) {
            puts("User pressed cancel.");
        } else {
            printf("Error: %s\n", NFD_GetError());
        }
        NFD_Quit();
    } else {
        SceneManagement::SceneManager::SaveScene();
    }

}

void KritiaEngine::Editor::GUI::MainMenuBarFunction::OpenSettingWindow() {


}