#include "Component.hpp"
#include "utils.hpp"

Component::Component(std::string name) : name(name) {
  std::string componentName = std::regex_replace(name, std::regex("\\s+"), "");
  printf("New Component Name %s", componentName.c_str());
};
