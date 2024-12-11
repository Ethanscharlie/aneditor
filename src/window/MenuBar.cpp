#include "MenuBar.hpp"
#include "../utils.hpp"

void menuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project")) {
        createProject("/tmp/newProject");
      }

      if (ImGui::MenuItem("Save")) {
        save();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
