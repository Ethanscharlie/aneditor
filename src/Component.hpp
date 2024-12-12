#pragma once

#include "Property.hpp"
#include <regex>
#include <vector>

struct Component {
  Component(std::string name);
  std::string name;
  std::vector<Property *> properties;
  std::string headerFile;
  std::string sourceFile;
};
