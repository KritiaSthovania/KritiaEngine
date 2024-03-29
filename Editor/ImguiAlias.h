#pragma once
#include <imgui/imgui.h>
#include <string>

namespace KritiaEngine::Editor::GUI {
	class ImguiAlias {
	public:
		static void FloatField(const char* label, float* value);
		static void FloatField3(const char* label, float* value);
		static void FloatField4(const char* label, float* value);
		static void IntField(const char* label, int* value);
		static void BoolField(const char* label, bool* value);
		static void ColorField3(const char* label, float* value);
		/// <summary>
		/// Open a window to select a file with a given postfix in the current asset folder.
		/// Return the path to the file.
		/// </summary>
		/// <param name="filePostfix">postfix with out the "."</param>
		static std::string OpenFindResourceWindow(const char* resourceType, const char* filePostfix);
		static std::string OpenSaveResourceWindow(const char* resourceType, const char* filePostfix, const char* defaultName);
		static int treeFlagDefaultOpenAndSpanFullWidth;
	};
}

