﻿#include "Main.h"
#include "CoreModule/Debug.h"
#include "Rendering/RenderManager.h"

using namespace KritiaEngine;
using namespace KritiaEngine::SceneManagement;

int main() 
{
    if (!InitializeWindow()) {
        return -1;
    }

    InitializeCoreModules();

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

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//渲染循环
void Render() {
    RenderManager::ClearFramebuffer();
    SceneManager::Render();
    glfwSwapBuffers(window);
}

void Update() {
    SceneManager::Update();
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

    window = glfwCreateWindow(Settings::ScreenWidth, Settings::ScreenHeight, "Kritia Engine", NULL, NULL);
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
    RenderManager::Initialize();
}
