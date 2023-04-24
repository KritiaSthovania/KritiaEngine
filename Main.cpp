#include "Main.h"
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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    Settings::Deserialize();
    MSG msg;
    WNDCLASS wndclass;
    //对窗口类的各属性进行初始化
    wndclass.style = CS_HREDRAW | CS_VREDRAW; /*窗口类的风格，CS前缀,C表示Class,S表示
                    Style，这里使用了水平和垂直风格*/
    wndclass.lpfnWndProc = WndProc;  /*这里将回到函数的名字赋值用以windows后面回调*/
    wndclass.cbClsExtra = 0;  //附加参数，通常情况下为0
    wndclass.cbWndExtra = 0;  //附加参数，通常情况下为0
    wndclass.hInstance = hInstance;  //窗口句柄，这里将WinMain中的hInstance句柄赋值就可
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); /*窗口图标，LoadIcon()是加载图标，这里是加载一个系统资源图标，LoadIcon()的原型是HICON LoadIcon(HINSTANCE, LPCSTR);*/
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  /*加载鼠标，同上相似*/
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  /*窗口画刷，这里是使用的白色画刷，所以创建出来的窗口的背景颜色则是白色的*/
    wndclass.lpszMenuName = NULL;  //窗口菜单名称，这里没有菜单，设为NULL
    wndclass.lpszClassName = TEXT("KritiaEngineWindow");  //窗口类名称，这个窗口类名称可作为这个窗口的唯一标识
    if (!RegisterClass(&wndclass)) {
        return 0;
    } 

    hwnd = CreateWindow(TEXT("KritiaEngineWindow"), TEXT("KritiaEngine"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Settings::ScreenWidth, Settings::ScreenHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    InitializeCoreModules();
    //InitializeGUI();
    startTime = SYSTEMTIME();
    GetLocalTime(&startTime);
    while (GetMessage(&msg, hwnd, NULL, 0)) {
        SYSTEMTIME time = SYSTEMTIME();
        GetLocalTime(&time);
        Time::UpdateDeltaTime(time.wMilliseconds - startTime.wMilliseconds);
        //执行一些特殊的输入控制
        //ProcessInput();
        //翻译消息
        TranslateMessage(&msg);
        //派发消息
        DispatchMessage(&msg);
        //执行所有Update函数，处理逻辑
        Update();
        //渲染
        Render();
        ResourceManager::CollectGarbage();
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //处理消息
    switch (message) {
    case WM_CREATE:
        /*窗口在创建时，会接收到该消息，通常在这里进行初始化操作*/
        return 0;
    case WM_SIZE:
        /*窗口被改变大小时，会接收到该消息，在窗口被创建时也会收到一次*/
        return 0;
    case WM_PAINT:
        /*窗口有绘图操作更新时,会收到这个消息*/
        return 0;
    case WM_DESTROY:
        /*关不窗口时，会收到该消息，PostQuitMessage()像系统表明终止当前线程，没有这个函数的话，窗口不会关闭*/
        PostQuitMessage(0);
        return 0;
    }
    //将不需要处理的消息传递给系统作默认处理
    return DefWindowProc(hwnd, message, wParam, lParam);
}


//渲染循环
void Render() {
    RenderingProvider::ClearFramebuffer();
    RendererManager::Render();
    if (Settings::UseOpenGL) {
        ImguiManager::RenderGUI();
    }
    RenderingProvider::SwapFramebuffer(window);
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
    if (Settings::UseOpenGL) {
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
    } else if (Settings::UseSoftwareRendering) {

    }
    return false;
}

void InitializeGUI() {
    ImguiManager::Initialize(window, editor);
}

void ProcessInput() {
    if (Settings::UseOpenGL) {
        if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
        //重置鼠标偏移
        Input::ResetMouseOffset();
        //检查事件，交换缓冲
        glfwPollEvents();
    } else if(Settings::UseSoftwareRendering){

    }

}

// 初始化所有核心模块
void InitializeCoreModules() {
    Input::Initialize(window);
    RenderingProvider::Initialize(hwnd);
    SceneManager::Initialize(editor);
    PhysicsManager::Initialize();
}

