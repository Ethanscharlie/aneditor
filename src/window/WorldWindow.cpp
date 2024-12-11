#include "WorldWindow.hpp"
#include "../Template.hpp"
#include "../TemplateInstance.hpp"
#include "../utils.hpp"
#include "imgui.h"
#include <cmath>

static float zoom = 4;

static void drawRect(int x, int y, int w, int h, ImColor color) {
  int realX = x;
  int realY = y;
  int realW = realX + w;
  int realH = realY + h;

  ImDrawList *drawList = ImGui::GetWindowDrawList();
  drawList->AddRectFilled(ImVec2(realX, realY), ImVec2(realW, realH), color);
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
      TemplateInstance instance =
          TemplateInstance(&templates[selectedTemplate]);
      instance.x = (int)(mouse_pos_in_canvas.x / zoom / 16.0) * 16;
      instance.y = (int)(mouse_pos_in_canvas.y / zoom / 16.0) * 16;
      templateInstances.push_back(instance);
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
    // for (int n = 0; n < points.Size; n += 2)
    //   draw_list->AddLine(
    //       ImVec2(origin.x + points[n].x, origin.y + points[n].y),
    //       ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y),
    //       IM_COL32(255, 255, 0, 255), 2.0f);
    // draw_list->PopClipRect();
    //

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
