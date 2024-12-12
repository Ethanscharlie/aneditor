#include "utils.hpp"
#include "Property.hpp"
#include "Template.hpp"
#include <any>
#include <format>
#include <fstream>
#include <iostream>

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

static void copy_directory(const fs::path &source,
                           const fs::path &destination) {
  try {
    // Check if source directory exists
    if (!fs::exists(source) || !fs::is_directory(source)) {
      std::cerr << "Source directory does not exist or is not a directory\n";
      return;
    }

    // Create destination directory if it does not exist
    if (!fs::exists(destination)) {
      fs::create_directory(destination);
    }

    // Iterate over the source directory and copy files and subdirectories
    for (const auto &entry : fs::directory_iterator(source)) {
      const auto &path = entry.path();
      auto dest = destination / path.filename(); // Destination path

      if (fs::is_directory(path)) {
        // Recursively copy subdirectories
        copy_directory(path, dest);
      } else if (fs::is_regular_file(path)) {
        // Copy file to destination
        fs::copy(path, dest, fs::copy_options::overwrite_existing);
      }
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

void createProject(const fs::path &destination) {
  copy_directory("res/projectTemplate", destination);
  projectPath = destination;
  std::cout << "Project Created Sucessfully\n";
}

void loadProject(fs::path _projectPath) {
  projectPath = _projectPath;
  fs::path datafile = projectPath / "res" / "EditorData.json";
  std::ifstream file(datafile);
  json editorData = json::parse(file);
  file.close();

  cameraScale = editorData["cameraScale"];
  zoom = editorData["zoom"];

  for (json &templateJson : editorData["Templates"]) {
    Template entityTemplate(templateJson["name"]);

    if (editorData.contains("mainComponent")) {
      entityTemplate.mainComponent = editorData["mainComponent"];
    }

    templates.push_back(entityTemplate);
  }

  for (json &instanceJson : editorData["instances"]) {
    std::string name = instanceJson["name"];

    Template *entityTemplate;
    for (Template &aTemplate : templates) {
      if (aTemplate.name == name) {
        entityTemplate = &aTemplate;
        break;
      }
    }

    if (!entityTemplate) {
      printf("Could not find template for %s\n", name.c_str());
      continue;
    }

    TemplateInstance instance(entityTemplate);
    instance.x = instanceJson["x"];
    instance.y = instanceJson["y"];

    templateInstances.push_back(instance);
  }
}

void save() {
  fs::path datafile = projectPath / "res" / "EditorData.json";
  json jsondata;

  jsondata["cameraScale"] = cameraScale;
  jsondata["zoom"] = zoom;

  for (Template temp : templates) {
    json templateJson;
    templateJson["name"] = temp.name;
    templateJson["mainComponent"] = temp.mainComponent;

    for (Property *property : temp.properties) {
      json propertyJson;
      propertyJson["name"] = property->name;

      switch (property->type) {
      case PropertyType::Decimal:
        propertyJson["value"] = std::any_cast<float>(property->value);
        propertyJson["type"] = "Decimal";
        break;
      case PropertyType::Integer:
        propertyJson["value"] = std::any_cast<int>(property->value);
        propertyJson["type"] = "Integer";
        break;
      case PropertyType::Checkbox:
        propertyJson["value"] = std::any_cast<bool>(property->value);
        propertyJson["type"] = "Checkbox";
        break;
      case PropertyType::Text:
        propertyJson["value"] = std::any_cast<std::string>(property->value);
        propertyJson["type"] = "Text";
        break;
      case PropertyType::Filepath:
        propertyJson = std::any_cast<std::string>(property->value);
        propertyJson["type"] = "Filepath";
        break;
      case PropertyType::Color:
        auto color = std::any_cast<std::array<float, 3>>(property->value);
        propertyJson["value"]["r"] = color[0];
        propertyJson["value"]["g"] = color[1];
        propertyJson["value"]["b"] = color[2];
        propertyJson["type"] = "Color";
        break;
      }

      templateJson["Properties"].push_back(propertyJson);
    }

    jsondata["Templates"].push_back(templateJson);
  }

  for (TemplateInstance &instance : templateInstances) {
    json instanceJson;
    instanceJson["name"] = instance.entityTempate->name;
    instanceJson["x"] = instance.x;
    instanceJson["y"] = instance.y;
    jsondata["instances"].push_back(instanceJson);
  }

  std::ofstream file(datafile);
  file << std::setw(2) << jsondata << std::endl;
  file.close();
  std::cout << std::format("Project save to {} Sucessfully\n",
                           datafile.string());
}

void createTemplate() {
  fs::path sourceFolder = projectPath / "src";

  Template entityTemplate("New Template");
  createComponent("NewTemplate");
  entityTemplate.mainComponent = "NewTemplate";
  templates.push_back(entityTemplate);
}

void createComponent(std::string name) {
  fs::path sourceFolder = projectPath / "src";

  std::ofstream headerFile(sourceFolder / std::format("{}.hpp", name));
  headerFile << "#pragma once" << std::endl;
  headerFile << "#include \"Charlie2D.hpp\"" << std::endl;
  headerFile << "#include \"engineUtils.hpp\"" << std::endl;
  headerFile << "" << std::endl;
  headerFile << "class " << name << " : public Component {" << std::endl;
  headerFile << "public:" << std::endl;
  headerFile << "  void start() override;" << std::endl;
  headerFile << "  void update(float deltaTime) override;" << std::endl;
  headerFile << "};" << std::endl;
  headerFile << "REGISTER_COMPONENT(" << name << ");" << std::endl;
  headerFile.close();

  std::ofstream sourceFile(sourceFolder / std::format("{}.cpp", name));
  sourceFile << "#include \"" << name << ".hpp\"" << std::endl;
  sourceFile << "" << std::endl;
  sourceFile << "void " << name << "::start() {}" << std::endl;
  sourceFile << "void " << name << "::update(float deltaTime) {}" << std::endl;
  sourceFile.close();
}
