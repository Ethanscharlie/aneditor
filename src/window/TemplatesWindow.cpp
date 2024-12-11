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
    ImGui::BeginChild("Templates Left Pane", ImVec2(150, 0),
                      ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    {
      for (int i = 0; i < templates.size(); i++) {
        if (ImGui::Selectable(templates[i].name.c_str(),
                              selectedTemplate == i)) {
          selectedTemplate = i;
        }
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginGroup();
    {
      ImGui::BeginChild("Templates Right Pane",
                        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
      ImGui::Text(templates[selectedTemplate].name.c_str());

      ImGui::Separator();

      ImGui::Text("Properties");
      auto &properties = templates[selectedTemplate].properties;

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
                             &std::any_cast<std::string &>(property.value), 128,
                             ImGuiInputTextFlags_None);
          break;
        }
        case PropertyType::Filepath: {
          ImGui::InputString(std::format("###String InputIn {}", i).c_str(),
                             &std::any_cast<std::string &>(property.value), 128,
                             ImGuiInputTextFlags_None);
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

      int dropdownSelection = 0;
      std::array<const char *, 7> dropdownOptions = {
          "Add a Property", "Decimal",  "Integer", "Checkbox",
          "Text",           "Filepath", "Color"};
      if (ImGui::Combo("### Add prop dropdown", &dropdownSelection,
                       dropdownOptions.data(), 7)) {
        if (dropdownSelection != 0) {
          Property *newProperty =
              new Property((PropertyType)(dropdownSelection - 1));
          templates[selectedTemplate].properties.push_back(newProperty);
        }
      }

      ImGui::Separator();

      ImGui::Text("Behaviour");

      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}
