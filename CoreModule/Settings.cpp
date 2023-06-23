#include "Settings.h"
#include "../Editor/EditorApplication.h"
#include "../Editor/ImguiAlias.h"
#include "SceneManager.h"
#include <json/json.hpp>
#include <fstream>
#include <imgui/imgui.h>

using namespace KritiaEngine;
using RenderingBackend = KritiaEngine::Rendering::RenderingProvider::RenderingBackend;
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;

int Settings::ScreenWidth = 3840;
int Settings::ScreenHeight = 2160;
float Settings::NearPlaneDistance = 0.1f;
float Settings::FarPlaneDistance = 100.f;
RenderingBackend Settings::renderingBackend = RenderingBackend::OpenGL;
bool Settings::UseGLM = true;
int Settings::ShadowWidth = 1024;
int Settings::ShadowHeight = 1024;
std::string Settings::LastOpenedScenePath = "";

void KritiaEngine::Settings::Serialize() {
    json json;
    json["LastOpenedScene"] = SceneManagement::SceneManager::GetActiveScene()->path;
    json["ScreenWidth"] = ScreenWidth;
    json["ScreenHeight"] = ScreenHeight;
    json["NearPlaneDistance"] = NearPlaneDistance;
    json["FarPlaneDistance"] = FarPlaneDistance;
    json["RenderingBackend"] = RenderingBackend::OpenGL;
    switch (renderingBackend) {
    case RenderingBackend::OpenGL:
        json["RenderingBackend"] = "OpenGL";
        break;
    case RenderingBackend::Vulkan:
        json["RenderingBackend"] = "Vulkan";
        break;
    case RenderingBackend::Software:
        json["RenderingBackend"] = "Software";
    }
    json["UseGLM"] = UseGLM;
    json["ShadowWidth"] = ShadowWidth;
    json["ShadowHeight"] = ShadowHeight;
    std::fstream output;
    output.open(Editor::EditorApplication::assetFolderRootPath + "Settings.json", std::ios::out | std::ios::trunc);
    output << json.dump() << std::endl;
    output.close();
}

void KritiaEngine::Settings::Deserialize() {
    std::ifstream input(Editor::EditorApplication::assetFolderRootPath + "Settings.json");
    json json = json::parse(input);
    LastOpenedScenePath = json["LastOpenedScene"];
    ScreenWidth = json["ScreenWidth"];
    ScreenHeight = json["ScreenHeight"];
    NearPlaneDistance = json["NearPlaneDistance"];
    FarPlaneDistance = json["FarPlaneDistance"];
    if (json["RenderingBackend"] == "OpenGL") {
        renderingBackend = RenderingBackend::OpenGL;
    } else if (json["RenderingBackend"] == "Vulkan") {
        renderingBackend = RenderingBackend::Vulkan;
    } else if (json["RenderingBackend"] == "Software") {
        renderingBackend = RenderingBackend::Software;
    } else {
        renderingBackend = RenderingBackend::OpenGL;
    }
    UseGLM = json["UseGLM"];
    ShadowWidth = json["ShadowWidth"];
    ShadowHeight = json["ShadowHeight"];
    input.close();
}

void KritiaEngine::Settings::ShowOnEditorWindow() {
    ImguiAlias::IntField("Screen Width", &ScreenWidth);
    ImguiAlias::IntField("Screen Height", &ScreenHeight);
    ImguiAlias::FloatField("Near Plane Distance", &NearPlaneDistance);
    ImguiAlias::FloatField("Far Plane Distance", &FarPlaneDistance);
    ImguiAlias::BoolField("Use GLM", &UseGLM);
    ImguiAlias::IntField("Shadow Width", &ShadowWidth);
    ImguiAlias::IntField("Shadow Height", &ShadowHeight);
    const char* preview = "";
    switch (renderingBackend) {
    case RenderingBackend::OpenGL:
        preview = "OpenGL";
        break;
    case RenderingBackend::Vulkan:
        preview = "Vulkan";
        break;
    case RenderingBackend::Software:
        preview = "Software";
        break;
    }
    ImGui::Text("RenderingBackend");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##RenderingBackend", preview)) {
        if (ImGui::Selectable("OpenGL")) {
            renderingBackend = RenderingBackend::OpenGL;
        }
        if (ImGui::Selectable("Vulkan")) {
            renderingBackend = RenderingBackend::Vulkan;
        }
        if (ImGui::Selectable("Software")) {
            renderingBackend = RenderingBackend::Software;
        }
        ImGui::EndCombo();
    }
}

