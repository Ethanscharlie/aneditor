#include "TilesWindow.hpp"
#include "../utils.hpp"
#include "imgui.h"
#include <SDL_image.h>
#include <SDL_render.h>
#include <filesystem>
#include <format>
#include <fstream>

static char newLayerName[50] = "";

static void layerListPanel() {
  ImGui::BeginChild("Tile Layer List Panel", ImVec2(150, 0),
                    ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
  {
    if (ImGui::Button("Create New Layer")) {
      ImGui::OpenPopup("New Layer Name Popup");
    }

    if (ImGui::BeginPopup("New Layer Name Popup")) {
      ImGui::InputText("Component Name", newLayerName,
                       IM_ARRAYSIZE(newLayerName));

      if (ImGui::Button("Create")) {
        if (strcmp(newLayerName, "")) {
          json newLayerJson;
          newLayerJson["name"] = newLayerName;
          newLayerJson["texturePath"] = "res/images/tiles.png";
          newLayerJson["tiles"] = json::array();
          selectedTileLayer = tileLayers.size();
          selectedTemplate = -1;

          tileLayers.push_back(newLayerJson);
          strcpy(newLayerName, "");
        }
      }

      ImGui::EndPopup();
    }

    if (tileLayers.size() > 0) {
      for (int i = 0; i < tileLayers.size(); i++) {
        std::string layerName = tileLayers[i]["name"];
        if (ImGui::Selectable(layerName.c_str(), selectedTileLayer == i)) {
          selectedTileLayer = i;
          selectedTemplate = -1;
        }
      }
    }
  }
  ImGui::EndChild();
}

static void layerInfoPanel() {
  std::string &layerName =
      tileLayers[selectedTileLayer]["name"].get_ref<std::string &>();
  std::string &rawTexturePath =
      tileLayers[selectedTileLayer]["texturePath"].get_ref<std::string &>();
  std::string texturePath = fs::path(projectPath) / rawTexturePath;

  ImGui::InputString("Layer Name", &layerName, 127, ImGuiInputTextFlags_None);

  if (ImGui::Button("Remove Layer")) {
    auto it = tileLayers.begin() + selectedTileLayer;
    tileLayers.erase(it);
    selectedTileLayer = 0;
    return;
  }

  ImGui::InputString("Tileset Path", &rawTexturePath, 127,
                     ImGuiInputTextFlags_None);

  SDL_Texture *texture = getTexture(texturePath);
  if (!texture) {
    return;
  }

  int textureWidth;
  int textureHeight;
  SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

  for (int tileY = 0; tileY < textureHeight; tileY += 16.0) {
    for (int tileX = 0; tileX < textureWidth; tileX += 16.0) {

      ImVec2 uv0 =
          ImVec2((float)tileX / textureWidth, (float)tileY / textureHeight);
      ImVec2 uv1 = ImVec2((float)(tileX + 16.0) / textureWidth,
                          (float)(tileY + 16.0) / textureHeight);

      std::string buttonId = std::format("Imagebutton{}{}", tileX, tileY);

      bool button = ImGui::ImageButton(buttonId.c_str(), (ImTextureID)texture,
                                       ImVec2(40, 40), uv0, uv1);

      if (button) {
        selectedTile["x"] = tileX;
        selectedTile["y"] = tileY;
        selectedTile["texture"] = texturePath;
      }

      ImGui::SameLine();
    }
    ImGui::Text("");
  }
}

void tilesWindow() {
  ImGui::Begin("Tiles");
  {
    layerListPanel();
    if (tileLayers.size() > 0) {
      ImGui::SameLine();

      ImGui::BeginChild("Layer Info Right Pane",
                        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
      layerInfoPanel();
      ImGui::EndChild();
    }
  }
  ImGui::End();
}
