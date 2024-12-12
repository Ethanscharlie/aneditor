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

#include "../Property.hpp"
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
    if (templates.size() > 0) {
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
        auto &properties = components[0].properties;

        ImGui::Separator();
        ImGui::Text(components[0].name.c_str());

        for (int i = 0; i < properties.size(); i++) {
          ImGui::TableNextColumn();
          Property &property = *properties[i];

          if (ImGui::Button(std::format("-###RemoveButton{}", i).c_str())) {
            std::vector<Property *>::iterator it = properties.begin() + i;
            properties.erase(it);
            std::cout << std::format("{}\n", properties.size());
            delete *it;
            i--;
            continue;
          }

          ImGui::SameLine();

          ImGui::SetNextItemWidth(100.0f);
          ImGui::InputString(std::format("###String Input {}", i).c_str(),
                             &property.name, 128, ImGuiInputTextFlags_None);

          ImGui::SameLine();

          switch (property.type) {
          case PropertyType::Decimal: {
            float &value = std::any_cast<float &>(property.value);
            ImGui::InputFloat(std::format("###PropValue{}", i).c_str(), &value);
            break;
          }
          case PropertyType::Integer: {
            ImGui::InputInt(std::format("###PropValue{}", i).c_str(),
                            &std::any_cast<int &>(property.value));
            break;
          }
          case PropertyType::Checkbox: {
            ImGui::Checkbox(std::format("###PropValueIn{}", i).c_str(),
                            &std::any_cast<bool &>(property.value));
            break;
          }
          case PropertyType::Text: {
            ImGui::InputString(std::format("###String InputIn {}", i).c_str(),
                               &std::any_cast<std::string &>(property.value),
                               128, ImGuiInputTextFlags_None);
            break;
          }
          case PropertyType::Filepath: {
            ImGui::InputString(std::format("###String InputIn {}", i).c_str(),
                               &std::any_cast<std::string &>(property.value),
                               128, ImGuiInputTextFlags_None);
            break;
          }
          case PropertyType::Color: {
            auto &color = std::any_cast<std::array<float, 3> &>(property.value);
            ImGui::ColorEdit3(std::format("###Color Input {}", i).c_str(),
                              color.data());
            break;
          }
          }
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}
