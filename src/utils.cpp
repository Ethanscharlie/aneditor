#include "utils.hpp"
#include "Component.hpp"
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

  for (json &componentJson : editorData["components"]) {
    Component newComponent = {componentJson["name"]};
    newComponent.properties = componentJson["properties"];
    components.push_back(newComponent);
  }

  for (json &templateJson : editorData["Templates"]) {
    Template *entityTemplate = new Template(templateJson["name"]);
    entityTemplate->components = templateJson["components"];

    if (editorData.contains("mainComponent")) {
      entityTemplate->components = editorData["components"];
    }

    templates.push_back(entityTemplate);
  }

  for (json &instanceJson : editorData["instances"]) {
    std::string name = instanceJson["name"];

    Template *entityTemplate;
    for (Template *aTemplate : templates) {
      if (aTemplate->name == name) {
        entityTemplate = aTemplate;
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

  for (Template *temp : templates) {
    json templateJson;
    templateJson["name"] = temp->name;
    templateJson["components"] = temp->components;
    jsondata["Templates"].push_back(templateJson);
  }

  for (TemplateInstance &instance : templateInstances) {
    json instanceJson;
    instanceJson["name"] = instance.entityTempate->name;
    instanceJson["x"] = instance.x;
    instanceJson["y"] = instance.y;
    jsondata["instances"].push_back(instanceJson);
  }

  for (Component &component : components) {
    json componentJson;
    componentJson["name"] = component.name;
    componentJson["properties"] = component.properties;
    jsondata["components"].push_back(componentJson);
  }

  std::ofstream file(datafile);
  file << std::setw(2) << jsondata << std::endl;
  file.close();
  std::cout << std::format("Project save to {} Sucessfully\n",
                           datafile.string());
}

void createTemplate() {
  fs::path sourceFolder = projectPath / "src";

  Template *entityTemplate = new Template("New Template");
  entityTemplate->mainComponent = "NewTemplate";
  templates.push_back(entityTemplate);
}

void createComponentFiles(std::string name) {
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

void createInput(json &property, std::string propertyType) {
  std::string i = property["name"];
  if (propertyType == "Decimal" && property["value"].is_number_float()) {
    double &value = property["value"].get_ref<double &>();
    ImGui::InputFloat(std::format("###PropValue{}", i).c_str(),
                      &(float &)value);
  }
  if (propertyType == "Integer" && property["value"].is_number_integer()) {
    int &value = (int &)property["value"].get_ref<std::int64_t &>();
    ImGui::InputInt(std::format("###PropValue{}", i).c_str(), &value);
  }
  if (propertyType == "Checkbox" && property["value"].is_boolean()) {
    bool &value = property["value"].get_ref<bool &>();
    ImGui::Checkbox(std::format("###PropValueIn{}", i).c_str(), &value);
  }
  if (propertyType == "Text" && property["value"].is_string()) {
    std::string &value = property["value"].get_ref<std::string &>();
    ImGui::InputString(std::format("###String InputIn {}", i).c_str(), &value,
                       128, ImGuiInputTextFlags_None);
  }
  if (propertyType == "Filepath" && property["value"].is_string()) {
    std::string &value = property["value"].get_ref<std::string &>();
    ImGui::InputString(std::format("###String InputIn {}", i).c_str(), &value,
                       128, ImGuiInputTextFlags_None);
  }
  if (propertyType == "Color") {
    json &value = property["value"];
    float &r = (float &)value["r"].get_ref<double &>();
    float &g = (float &)value["g"].get_ref<double &>();
    float &b = (float &)value["b"].get_ref<double &>();
    std::array<float *, 3> color = {&r, &g, &b};
    ImGui::ColorEdit3(std::format("###Color Input {}", i).c_str(),
                      *color.data());
  }
}
