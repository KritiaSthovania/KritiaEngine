#include "Input.h"

using namespace KritiaEngine;

GLFWwindow* Input::window = nullptr;
bool Input::mouseInitialized = false;
double Input::lastX = Settings::ScreenWidth / 2.0;
double Input::lastY = Settings::ScreenHeight / 2.0;
double Input::mouseXoffset = 0.0;
double Input::mouseYoffset = 0.0;
double Input::mouseScrollOffset = 0.0;

void KritiaEngine::Input::Initialize(GLFWwindow* window)
{
	Input::window = window;
}

bool KritiaEngine::Input::GetKeyDown(int glfwKey)
{
	return glfwGetKey(window, glfwKey) == GLFW_PRESS;
}

bool KritiaEngine::Input::GetMouseButtonDown(int glfwMouseButton) {
    return glfwGetMouseButton(window, glfwMouseButton) == GLFW_PRESS;
}

void KritiaEngine::Input::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseInitialized) {
        lastX = xpos;
        lastY = ypos;
        mouseInitialized = true;
    }
    mouseXoffset = xpos - lastX;
    mouseYoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;
}

void KritiaEngine::Input::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    mouseScrollOffset = yoffset;
}


double KritiaEngine::Input::GetMouseXOffset()
{
    return mouseXoffset;
}

double KritiaEngine::Input::GetMouseYOffset()
{
    return mouseYoffset;
}

void KritiaEngine::Input::ResetMouseOffset()
{
    mouseXoffset = 0.0;
    mouseYoffset = 0.0;
    mouseScrollOffset = 0.0;
}

GLFWwindow* KritiaEngine::Input::GetWindow()
{
    return window;
}

double KritiaEngine::Input::GetMouseScrollOffset()
{
    return mouseScrollOffset;
}
