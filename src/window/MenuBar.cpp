#include "MenuBar.hpp"
#include "../utils.hpp"
#include <cstdlib>
#include <format>

void menuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project")) {
        createProject("/home/ethanscharlie/newProject");
      }

      if (ImGui::MenuItem("Save")) {
        save();
      }

      if (ImGui::MenuItem("Run")) {
        std::string command = std::format("cd {}; cd build; cmake ..; make; ./index",
                                projectPath.string());
        std::system(command.c_str());
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
