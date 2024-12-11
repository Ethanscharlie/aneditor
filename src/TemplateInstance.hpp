#pragma once

#include "Template.hpp"

struct TemplateInstance {
  TemplateInstance(Template *entityTempate) : entityTempate(entityTempate) {}
  Template *entityTempate;
  float x = 0;
  float y = 0;
};
