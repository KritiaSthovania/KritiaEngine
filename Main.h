#pragma once
#include "pch.h"
#include <stb_image.h>
#include "CoreModule/Input.h"
#include "CoreModule//SceneManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
bool InitializeWindow();
void ProcessInput();
void Render();
void Update();
void InitializeCoreModules();

constexpr bool editor = true;

GLFWwindow* window;


