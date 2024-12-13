#pragma once

#include <regex>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Component {
  Component(std::string name);
  std::string name;
  std::string headerFile;
  std::string sourceFile;

  json properties;
};
