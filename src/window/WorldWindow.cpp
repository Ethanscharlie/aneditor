#include "WorldWindow.hpp"
#include "../Template.hpp"
#include "../TemplateInstance.hpp"
#include "../utils.hpp"
#include "imgui.h"
#include <cmath>
#include <cstdio>

static void drawRect(int x, int y, int w, int h, ImColor color) {
  int realX = x;
  int realY = y;
  int realW = realX + w;
  int realH = realY + h;

  ImDrawList *drawList = ImGui::GetWindowDrawList();
  drawList->AddRectFilled(ImVec2(realX, realY), ImVec2(realW, realH), color,
                          30);
}

void worldWindow() {
  ImGui::Begin("World");
  {
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
                                                    // screen coordinates!
    ImVec2 canvas_sz =
        ImGui::GetContentRegionAvail(); // Resize canvas to what's available
    if (canvas_sz.x < 50.0f)
      canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f)
      canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 =
        ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz,
                           ImGuiButtonFlags_MouseButtonLeft |
                               ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x,
                        canvas_p0.y + scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                     io.MousePos.y - origin.y);

    // Add first and second point
    if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      int gridX = floor(mouse_pos_in_canvas.x / zoom / 16.0) * 16;
      int gridY = floor(mouse_pos_in_canvas.y / zoom / 16.0) * 16;
      float mx = mouse_pos_in_canvas.x;
      float my = mouse_pos_in_canvas.y;

      if (selectedTemplate != -1) {
        TemplateInstance *hoverInstance = nullptr;
        int hoverInstanceIndex = -1;

        for (int i = 0; i < templateInstances.size(); i++) {
          TemplateInstance &instance = templateInstances[i];
          float x = instance.x * zoom;
          float y = instance.y * zoom;
          float w = instance.entityTempate->width * zoom;
          float h = instance.entityTempate->height * zoom;

          if (mx > x && mx < x + w && my > y && my < y + h) {
            hoverInstance = &instance;
            hoverInstanceIndex = i;
          }
        }

        if (hoverInstance != nullptr) {
          templateInstances.erase(templateInstances.begin() +
                                  hoverInstanceIndex);
        }

        else {

          TemplateInstance instance =
              TemplateInstance(templates[selectedTemplate]);
          instance.x = gridX;
          instance.y = gridY;
          templateInstances.push_back(instance);
        }
      }

      else if (selectedTileLayer != -1) {
        bool hoveringHoverTile = false;

        for (json &tileLayer : tileLayers) {
          json &tiles = tileLayer["tiles"];
          for (json &tile : tiles) {
            int x = (int)tile["x"] * zoom;
            int y = (int)tile["y"] * zoom;
            int w = 16 * zoom;
            int h = 16 * zoom;

            if (mx > x && mx < x + w && my > y && my < y + h) {
              hoveringHoverTile = true;
              auto it = std::find(tiles.begin(), tiles.end(), tile);
              tiles.erase(it);
              break;
            }
          }
        }

        if (!hoveringHoverTile) {
          json &tileLayer = tileLayers[selectedTileLayer];
          json &tiles = tileLayer["tiles"];

          json newTileJson;
          newTileJson["x"] = gridX;
          newTileJson["y"] = gridY;
          tiles.push_back(newTileJson);
        }
      }
    }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the
    // mouse is hovering something etc.
    const float mouse_threshold_for_pan =
        opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right,
                                            mouse_threshold_for_pan)) {
      scrolling.x += io.MouseDelta.x;
      scrolling.y += io.MouseDelta.y;
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid) {
      const float GRID_STEP = 16.0f * zoom;
      for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
           x += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                           ImVec2(canvas_p0.x + x, canvas_p1.y),
                           IM_COL32(200, 200, 200, 40));
      for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
           y += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                           ImVec2(canvas_p1.x, canvas_p0.y + y),
                           IM_COL32(200, 200, 200, 40));
    }

    // Camera lines
    int cameraX = -screenWidth / 2.0 * zoom / cameraScale;
    int cameraY = -screenHeight / 2.0 * zoom / cameraScale;
    int cameraW = screenWidth * zoom / cameraScale;
    int cameraH = screenHeight * zoom / cameraScale;

    draw_list->AddLine(ImVec2(cameraX + origin.x, cameraY + origin.y),
                       ImVec2(origin.x + cameraX + cameraW, origin.y + cameraY),
                       IM_COL32(255, 10, 10, 200));
    draw_list->AddLine(
        ImVec2(cameraX + origin.x + cameraW, cameraY + origin.y),
        ImVec2(origin.x + cameraX + cameraW, origin.y + cameraY + cameraH),
        IM_COL32(255, 10, 10, 200));
    draw_list->AddLine(
        ImVec2(cameraX + origin.x + cameraW, cameraY + origin.y + cameraH),
        ImVec2(origin.x + cameraX, origin.y + cameraY + cameraH),
        IM_COL32(255, 10, 10, 200));
    draw_list->AddLine(ImVec2(cameraX + origin.x, cameraY + origin.y),
                       ImVec2(origin.x + cameraX, origin.y + cameraY + cameraH),
                       IM_COL32(255, 10, 10, 200));

    for (json &tileLayer : tileLayers) {
      json &tiles = tileLayer["tiles"];
      for (json &tile : tiles) {
        int x = (int)tile["x"] * zoom + origin.x;
        int y = (int)tile["y"] * zoom + origin.y;
        int w = 16 * zoom;
        int h = 16 * zoom;

        ImColor color = {0, 0, 0};
        drawRect(x, y, w, h, color);
      }
    }

    // Entity instances
    for (TemplateInstance instance : templateInstances) {
      int x = instance.x * zoom + origin.x;
      int y = instance.y * zoom + origin.y;
      int w = instance.entityTempate->width * zoom;
      int h = instance.entityTempate->height * zoom;

      ImColor color = {
          instance.entityTempate->editorColor[0],
          instance.entityTempate->editorColor[1],
          instance.entityTempate->editorColor[2],
      };

      drawRect(x, y, w, h, color);
    }
  }
  ImGui::End();
}
