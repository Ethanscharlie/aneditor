#pragma once
#include "Template.hpp"
#include "TemplateInstance.hpp"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

using json = nlohmann::json;

inline fs::path projectPath = "";
inline int selectedTemplate;
inline std::vector<Template> templates = {{"Player"}, {"Slime"}, {"Chest"}};
inline std::vector<TemplateInstance> templateInstances;

namespace ImGui {
bool InputString(const char *label, std::string *strPtr, size_t bufferSize,
                 ImGuiInputTextFlags flags);
} // namespace ImGui

void createProject(const fs::path &destination);

void save();
