#include "ComponentsWindow.hpp"
#include "../utils.hpp"
#include "detail/value_t.hpp"
#include "imgui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <any>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <pstl/glue_algorithm_defs.h>
#include <string>
#include <vector>

static char newComponentName[50] = "";
static int selectedComponent;

void componentsWindow() {
  ImGui::Begin("Components");
  {
    if (ImGui::Button("Create New Component")) {
      ImGui::OpenPopup("New Component Name Popup");
    }

    if (ImGui::BeginPopup("New Component Name Popup")) {
      ImGui::InputText("Component Name", newComponentName,
                       IM_ARRAYSIZE(newComponentName));

      if (ImGui::Button("Create")) {
        if (strcmp(newComponentName, "")) {
          components.push_back({newComponentName});
          strcpy(newComponentName, "");
        }
      }

      ImGui::EndPopup();
    }

    ImGui::BeginChild("Components Left Pane", ImVec2(150, 0),
                      ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    {
      if (components.size() > 0) {
        for (int i = 0; i < components.size(); i++) {
          if (ImGui::Selectable(components[i].name.c_str(),
                                selectedComponent == i)) {
            selectedComponent = i;
          }
        }
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginGroup();
    if (components.size() > 0) {
      ImGui::BeginChild("components Right Pane",
                        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
      ImGui::Text(components[selectedComponent].name.c_str());

      if (ImGui::Button("Remove Template")) {
        auto it = components.begin() + selectedComponent;
        components.erase(it);
        selectedComponent = 0;
      }

      else {
        ImGui::Text("Properties");
        json &properties = components[selectedComponent].properties;

        for (int i = 0; i < properties.size(); i++) {
          ImGui::TableNextColumn();
          json &property = properties[i];

          if (ImGui::Button(std::format("-###RemoveButton{}", i).c_str())) {
            auto it = properties.begin() + i;
            properties.erase(it);
            std::cout << std::format("{}\n", properties.size());
            i--;
            continue;
          }

          ImGui::SameLine();

          std::string &propertyName = property["name"].get_ref<std::string &>();
          ImGui::SetNextItemWidth(100.0f);
          ImGui::InputString(std::format("###String Input {}", i).c_str(),
                             &propertyName, 128, ImGuiInputTextFlags_None);

          ImGui::SameLine();

          std::string propertyType = property["type"];
          createInput(property, propertyType);
        }

        int dropdownSelection = 0;
        std::array<const char *, 7> dropdownOptions = {
            "Add a Property", "Decimal",  "Integer", "Checkbox",
            "Text",           "Filepath", "Color"};
        if (ImGui::Combo("### Add prop dropdown", &dropdownSelection,
                         dropdownOptions.data(), 7)) {
          if (dropdownSelection != 0) {
            json propertyJson;
            propertyJson["name"] = "NewProperty";
            propertyJson["type"] = dropdownOptions[dropdownSelection];

            std::string propertyType = propertyJson["type"];
            if (propertyType == "Decimal") {
              propertyJson["value"] = 0.0;
            }
            if (propertyType == "Integer") {
              propertyJson["value"] = 0;
            }
            if (propertyType == "Checkbox") {
              propertyJson["value"] = false;
            }
            if (propertyType == "Text") {
              propertyJson["value"] = "";
            }
            if (propertyType == "Filepath") {
              propertyJson["value"] = "";
            }
            if (propertyType == "Color") {
              propertyJson["value"]["r"] = 255.0;
              propertyJson["value"]["g"] = 255.0;
              propertyJson["value"]["b"] = 255.0;
            }

            components[selectedComponent].properties.push_back(propertyJson);
          }
        }

        ImGui::Separator();

        ImGui::Text("Behaviour");
        ImGui::EndChild();
      }
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}
