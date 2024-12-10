#include "TemplatesWindow.hpp"

#include "../utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

struct Property {
  std::string name = "";
  int value = 0;
};

struct Template {
  Template(std::string name) : name(name) {}
  std::string name;
  std::vector<Property> properties;
};

static int selectedTemplate;
std::vector<Template> templates = {{"Player"}, {"Slime"}, {"Chest"}};

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
      for (int i = 0; i < templates[selectedTemplate].properties.size(); i++) {
        Property &property = templates[selectedTemplate].properties[i];

        ImGui::TableNextColumn();
        ImGui::InputString(std::format("###String Input {}", i).c_str(),
                           &property.name, 128, ImGuiInputTextFlags_None);
        ImGui::SameLine();
        ImGui::InputInt(std::format("###PropValue{}", i).c_str(),
                        &property.value);
      }

      if (ImGui::Button("         +         ")) {
        templates[selectedTemplate].properties.push_back(Property());
      }

      ImGui::Separator();

      ImGui::Text("Behaviour");

      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}
