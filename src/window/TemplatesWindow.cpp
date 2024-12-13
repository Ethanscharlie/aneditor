#include "TemplatesWindow.hpp"

#include "../utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <any>
#include <cstdint>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <pstl/glue_algorithm_defs.h>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "../Template.hpp"

void templatesWindow() {
  ImGui::Begin("Templates");
  {
    if (ImGui::Button("Create Template")) {
      createTemplate();
    }

    ImGui::BeginChild("Templates Left Pane", ImVec2(150, 0),
                      ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    {
      if (templates.size() > 0) {
        for (int i = 0; i < templates.size(); i++) {
          if (ImGui::Selectable(templates[i]->name.c_str(),
                                selectedTemplate == i)) {
            selectedTemplate = i;
          }
        }
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginGroup();
    if (templates.size() > 0 && selectedTemplate != -1) {
      auto &templateComponents = templates[selectedTemplate]->components;

      ImGui::BeginChild("Templates Right Pane",
                        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
      ImGui::InputString("###Template name input",
                         &templates[selectedTemplate]->name, 128,
                         ImGuiInputTextFlags_None);

      if (ImGui::Button("Remove Template")) {
        auto it = templates.begin() + selectedTemplate;
        templates.erase(it);
        delete *it;
        selectedTemplate = 0;
      }

      else {
        auto &editorColor = templates[selectedTemplate]->editorColor;
        ImGui::ColorEdit3("###EditorColorSelect", editorColor.data());

        ImGui::InputFloat("Width", &templates[selectedTemplate]->width);
        ImGui::InputFloat("Height", &templates[selectedTemplate]->height);

        ImGui::Separator();

        ImGui::Text("Components");
        ImGui::SameLine();
        if (ImGui::Button("Add Component")) {
          ImGui::OpenPopup("Add Component Name Popup");
        }

        if (ImGui::BeginPopup("Add Component Name Popup")) {
          int selectedIndex = -1;

          for (int i = 0; i < components.size(); i++) {
            Component &globalComponent = components[i];
            bool alreadyHasComponent = false;

            for (json &componentJson : templateComponents) {
              const std::string &componentName = componentJson["name"];
              if (componentName == globalComponent.name) {
                alreadyHasComponent = true;
              }
            }

            if (alreadyHasComponent)
              continue;

            if (ImGui::Selectable(globalComponent.name.c_str())) {
              selectedIndex = i;
            }
          }

          if (selectedIndex != -1) {
            json newComponentJson;
            newComponentJson["name"] = components[selectedIndex].name;
            newComponentJson["properties"] = json::array();
            templateComponents.push_back(newComponentJson);
          }

          ImGui::EndPopup();
        }

        for (int componentI = 0; componentI < templateComponents.size();
             componentI++) {
          json &componentJson = templateComponents[componentI];
          const std::string &componentName = componentJson["name"];
          json &properties = componentJson["properties"];

          ImGui::Separator();
          ImGui::Text(componentName.c_str());

          ImGui::SameLine();
          if (ImGui::Button(
                  std::format("Remove Component {}", componentName).c_str())) {
            auto it = templateComponents.begin() + componentI;
            templateComponents.erase(it);
          }

          // Find the global component
          auto it = std::find_if(components.begin(), components.end(),
                                 [&componentName](const Component &component) {
                                   return component.name == componentName;
                                 });
          Component &globalComponent = *it;

          // Ensure it has all of the proper components
          for (const json &globalProperty : globalComponent.properties) {
            bool componentAlreadyHasProperty = false;

            for (const json &property : properties) {
              if (property["name"] == globalProperty["name"]) {
                componentAlreadyHasProperty = true;
                break;
              }
            }

            if (!componentAlreadyHasProperty)
              properties.push_back(globalProperty);
          }

          // Trash removed properties
          for (int i = 0; i < properties.size(); i++) {
            const json &property = properties[i];
            bool globalProptertyExists = false;

            for (const json &globalProperty : globalComponent.properties) {
              if (property["name"] == globalProperty["name"]) {
                globalProptertyExists = true;
                break;
              }
            }

            if (!globalProptertyExists) {
              auto it = properties.begin() + i;
              properties.erase(it);
            }
          }

          for (int i = 0; i < properties.size(); i++) {
            ImGui::TableNextColumn();
            json &property = properties[i];

            ImGui::Text(property["name"].get<std::string>().c_str());

            ImGui::SameLine();

            createInput(property, property["type"]);
          }
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}
