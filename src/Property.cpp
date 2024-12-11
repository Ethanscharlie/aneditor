#include "Property.hpp"

Property::Property(PropertyType type) : type(type) {
  switch (type) {
  case PropertyType::Decimal:
    value = (float)0.0;
    break;
  case PropertyType::Integer:
    value = (int)0;
    break;
  case PropertyType::Checkbox:
    value = false;
    break;
  case PropertyType::Text:
    value = (std::string) "";
    break;
  case PropertyType::Filepath:
    value = (std::string) "";
    break;
  case PropertyType::Color:
    value = std::array<float, 3>({255, 255, 255});
    break;
  }
}
