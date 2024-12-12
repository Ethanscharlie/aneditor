#pragma once
#include "Component.hpp"
#include "Template.hpp"
#include "TemplateInstance.hpp"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

using json = nlohmann::json;

inline fs::path projectPath;
inline int selectedTemplate;
inline std::vector<Template *> templates;
inline std::vector<TemplateInstance> templateInstances;
inline std::vector<Component> components = {{"DefaultComp"}};
inline float zoom = 4;
inline int screenWidth = 1920;
inline int screenHeight = 1080;
inline float cameraScale = 2;

namespace ImGui {
bool InputString(const char *label, std::string *strPtr, size_t bufferSize,
                 ImGuiInputTextFlags flags);
} // namespace ImGui

void createProject(const fs::path &destination);

void loadProject(fs::path _projectPath);
void save();

void createTemplate();

void createComponent(std::string name);
