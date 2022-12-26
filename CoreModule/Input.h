#pragma once
#include "pch.h"
#include "Settings.h"
#include "Time.h"
namespace KritiaEngine{
	class Input
	{
	public:
		static void Initialize(GLFWwindow* window);
		static bool GetKeyDown(int glfwKey);
		static bool GetMouseButtonDown(int glfwMouseButton);
		static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static double GetMouseXOffset();
		static double GetMouseYOffset();
		static void ResetMouseOffset();
		static GLFWwindow* GetWindow();
		static double GetMouseScrollOffset();
	private:
		//当前所处的窗口
		static GLFWwindow* window;
		static bool mouseInitialized;
		static double lastX;
		static double lastY;
		static double mouseXoffset;
		static double mouseYoffset;
		//Y轴的scrollOffset
		static double mouseScrollOffset;
	};
}



