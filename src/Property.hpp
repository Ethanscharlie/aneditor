#pragma once

#include <any>
#include <array>
#include <string>

enum class PropertyType {
  Decimal = 0,
  Integer = 1,
  Checkbox = 2,
  Text = 3,
  Filepath = 4,
  Color = 5
};

struct Property {
  Property(PropertyType type);

  std::string name = "";
  const PropertyType type;
  std::any value;
};
