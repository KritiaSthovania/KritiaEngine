#pragma once
#include <imgui/imgui.h>
#include <string>
namespace KritiaEngine::Editor::GUI {
	class ImguiAlias {
	public:
		static void FloatField(const char* label, float* value);
		static void FloatField3(const char* label, float* value);
		static void FloatField4(const char* label, float* value);
		static void BoolField(const char* label, bool* value);
		static void ColorField3(const char* label, float* value);
		static int treeFlagDefaultOpenAndSpanFullWidth;
	};
}

