#pragma once
#include <pch.h>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
bool InitializeWindow();
void ProcessInput();
void Render();
void Update();
void InitializeCoreModules();

constexpr bool editor = true;

GLFWwindow* window;


