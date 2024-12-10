#include "utils.hpp"

namespace ImGui {
bool InputString(const char *label, std::string *strPtr, size_t bufferSize,
                 ImGuiInputTextFlags flags) {
  if (strPtr == nullptr) {
    return false;
  }

  char buffer[256];
  strncpy(buffer, strPtr->c_str(), bufferSize);
  buffer[bufferSize - 1] = '\0'; // Ensure null-terminated

  if (ImGui::InputText(label, buffer, bufferSize, flags)) {
    *strPtr = buffer;
    return true;
  }
  return false;
}
} // namespace ImGui
