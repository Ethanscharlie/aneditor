#pragma once
#include "Component.hpp"
#include "imgui.h"
#include <array>
#include <string>
#include <vector>

struct Property;

struct Template {
  Template(std::string name);
  ~Template();

  std::string name;
  std::string mainComponent;
  float width = 16;
  float height = 16;
  std::array<float, 3> editorColor = {255, 255, 255};
  json components;
};
