#pragma once
#include "imgui.h"
#include <string>

namespace ImGui {
bool InputString(const char *label, std::string *strPtr, size_t bufferSize,
                 ImGuiInputTextFlags flags);
} // namespace ImGui
