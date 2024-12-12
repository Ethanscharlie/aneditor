#include "Template.hpp"
#include "Property.hpp"
#include "utils.hpp"

Template::Template(std::string name) : name(name) {}

Template::~Template() {
  for (int i = 0; i < templateInstances.size(); i++) {
    auto &instance(templateInstances[i]);
    if (instance.entityTempate->name == templates[selectedTemplate]->name) {
      auto it = templateInstances.begin() + i;
      templateInstances.erase(it);
      i--;
    }
  }

  for (Property *property : properties) {
    delete property;
  }

  properties.clear();
}
