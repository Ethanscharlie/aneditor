#include "EditorSettings.hpp"
#include "imgui.h"
#include "../utils.hpp"

void editorSettings() {
  ImGui::Begin("Editor");
  {
    ImGui::SliderFloat("Zoom", &zoom, 1, 20);
    ImGui::SliderFloat("Camera Scale", &cameraScale, 0, 10);
  }
  ImGui::End();
}
