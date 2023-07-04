#pragma once
#define GLFW_INCLUDE_VULKAN
#include <pch.h>
#include <windows.h>
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
bool InitializeWindow();
void InitializeGUI();
void ProcessInput();
void Render();
void Update();
void InitializeCoreModules();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

constexpr bool editor = true;

GLFWwindow* window;
HWND hwnd;
SYSTEMTIME startTime;


