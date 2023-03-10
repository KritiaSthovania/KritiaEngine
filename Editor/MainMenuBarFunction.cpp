#include "MainMenuBarFunction.h"
#include "EditorApplication.h"
#include "../CoreModule/SceneManager.h"
#include <nfd/nfd.h>
#include <stdio.h>

void KritiaEngine::Editor::GUI::MainMenuBarFunction::OpenScene() {
    NFD_Init();

    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[2] = { { "Scene File", "scene.json"}};
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
