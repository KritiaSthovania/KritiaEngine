﻿#include "Main.h"
#include "Rendering/RenderingProvider.h"
#include "CoreModule/Input.h"
#include "CoreModule/SceneManager.h"
#include "CoreModule/Manager/RendererManager.h"
#include "CoreModule/Settings.h"
#include "Editor/ImguiManager.h"
#include "CoreModule/Manager/ResourceManager.h"
#include "CoreModule/Manager/PhysicsManager.h"
#include "Editor/EditorApplication.h"

using namespace KritiaEngine;
using namespace KritiaEngine::SceneManagement;
using namespace KritiaEngine::Manager;
using namespace KritiaEngine::Rendering;
using namespace KritiaEngine::Editor::GUI;
using namespace KritiaEngine::Editor;

constexpr const char* title = "Kritia Engine";

int main() 
{
    Settings::Deserialize();
    if (!InitializeWindow()) {
        return -1;
    }
    InitializeCoreModules();
    InitializeGUI();
    // 游戏循环, Tick
    while (!glfwWindowShouldClose(window)) 
    {
        Time::UpdateDeltaTime(glfwGetTime());
        //执行一些特殊的输入控制
        ProcessInput();
        //执行所有Update函数，处理逻辑
        Update();
        //渲染
        Render();
        ResourceManager::CollectGarbage();
    }
    Settings::Serialize();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//渲染循环
void Render() {
    RenderingProvider::ClearFramebuffer();
    RendererManager::Render();
    ImguiManager::RenderGUI();
    glfwSwapBuffers(window);
}

void Update() {
    if (editor) {
        Camera::editorCamera->EditorCameraUpdate();
    }
    BehaviourManager::ComponentUpdate();
    if (EditorApplication::isPlaying) {
        BehaviourManager::BehaviourUpdate();
        PhysicsManager::PhysicsUpdate();
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool InitializeWindow()
{
    //初始化窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (RenderingProvider::msaaEnabled) {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    window = glfwCreateWindow(Settings::ScreenWidth, Settings::ScreenHeight, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, Input::MouseCursorPosCallback);
    glfwSetScrollCallback(window, Input::MouseScrollCallback);

    glfwSwapInterval(1);
    return true;
}

void InitializeGUI() {
    ImguiManager::Initialize(window, editor);
}

void ProcessInput() {
    if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    //重置鼠标偏移
    Input::ResetMouseOffset();
    //检查事件，交换缓冲
    glfwPollEvents();
}

// 初始化所有核心模块
void InitializeCoreModules() {
    Input::Initialize(window);
    SceneManager::Initialize(editor);
    RenderingProvider::Initialize();
    PhysicsManager::Initialize();
}

