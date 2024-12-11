/*
 * This is meant to be for quick variable tweaking without having to recompile
 */

#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
typedef nlohmann::json json;

json loadConfigFile() {
  std::ifstream file("res/config.json");
  json out = json::parse(file);
  file.close();
  return out;
}

json configJson = loadConfigFile();
