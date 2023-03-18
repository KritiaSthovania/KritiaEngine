#include "Settings.h"
#include "../Editor/EditorApplication.h"
#include "../Editor/ImguiAlias.h"
#include <json/json.hpp>
#include <fstream>
#include <imgui/imgui.h>

using namespace KritiaEngine;
using ImguiAlias = KritiaEngine::Editor::GUI::ImguiAlias;

int Settings::ScreenWidth = 3840;
int Settings::ScreenHeight = 2160;
float Settings::NearPlaneDistance = 0.1f;
float Settings::FarPlaneDistance = 100.f;
bool Settings::UseOpenGL = true;
bool Settings::UseGLM = true;
int Settings::ShadowWidth = 1024;
int Settings::ShadowHeight = 1024;

void KritiaEngine::Settings::Serialize() {
    json json;
    json["ScreenWidth"] = ScreenWidth;
    json["ScreenHeight"] = ScreenHeight;
    json["NearPlaneDistance"] = NearPlaneDistance;
    json["FarPlaneDistance"] = FarPlaneDistance;
    json["UseOpenGL"] = UseOpenGL;
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
    ScreenWidth = json["ScreenWidth"];
    ScreenHeight = json["ScreenHeight"];
    NearPlaneDistance = json["NearPlaneDistance"];
    FarPlaneDistance = json["FarPlaneDistance"];
    UseOpenGL = json["UseOpenGL"];
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
    ImguiAlias::BoolField("Use OpenGL", &UseOpenGL);
    ImguiAlias::BoolField("Use GLM", &UseGLM);
    ImguiAlias::IntField("Shadow Width", &ShadowWidth);
    ImguiAlias::IntField("Shadow Height", &ShadowHeight);
}
