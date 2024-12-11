#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <format>
#include <iostream>
#include <vector>

#include "ImGuiTheme.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "window/MenuBar.hpp"
#include "window/TemplatesWindow.hpp"

SDL_Window *window;
SDL_Renderer *renderer;

void initSDL() {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);

  window = SDL_CreateWindow("AnEditor", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, 1820, 980,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImFont *customFont =
      io.Fonts->AddFontFromFileTTF("res/Roboto-Regular.ttf", 30.0f);
  ;
}

int main(int argc, char *argv[]) {
  initSDL();

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      ImGui_ImplSDL2_ProcessEvent(&e);

      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.type != SDL_MOUSEMOTION) {
      }
    }

    SDL_RenderClear(renderer);

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    applyTheme();

    // Create the dockspace
    ImGui::PushStyleColor(
        ImGuiCol_DockingEmptyBg,
        ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background
    ImGui::PushStyleColor(
        ImGuiCol_WindowBg,
        ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background for
    ImGui::PushStyleColor(
        ImGuiCol_Border,
        ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent border color
    ImGuiID dockspaceId =
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::PopStyleColor(3); // Pop the three style colors that were pushed

    menuBar();
    templatesWindow();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
