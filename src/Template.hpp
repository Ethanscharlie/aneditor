#pragma once
#include <string>
#include <vector>

struct Property;

struct Template {
  Template(std::string name);
  std::string name;
  std::vector<Property *> properties;
};

