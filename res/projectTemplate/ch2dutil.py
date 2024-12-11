#!/usr/bin/env python 

import os
import sys

"""
This is a utilitys script for charlie2D, meant for generating files from templates
"""

HELP_DISPLAY = """This is a utilitys script for charlie2D, meant for generating files from templates

new_creater [name]
new_component [name]
new_moder [name]
"""

SRC_DIRECTORY = os.path.join(os.path.dirname(os.path.realpath(__file__)), "src")
NEW_COMPONENT_TEMPLATE = """#pragma once

#include "Charlie2D.hpp"

class {} : public Component {{
public:
  void start() override;
  void update(float deltaTime) override;
}};
"""
NEW_COMPONENT_TEMPLATE_CPP = """#include "{}_component.hpp"

void {}::start() {{
}}

void {}::update(float deltaTime) {{
}}
"""

NEW_CREATER_TEMPLATE = """#pragma once

#include "Charlie2D.hpp"

class {} : public Component {{
public:
  void start() override;
  void update(float deltaTime) override;
  static void createInstance(Vector2f centerPosition = {{0, 0}});
  static void configureInstance(Entity* entity);
}};

"""
NEW_CREATER_TEMPLATE_CPP = """#include "{}_mce.hpp"

#define ENTITY_TAG "{{}}"
#define IMAGE_FILE ""
#define SIZE {{16, 16}}

void {}::start() {{}}

void {}::update(float deltaTime) {{}}

void {}::createInstance(Vector2f centerPosition) {{
  Entity *entity = GameManager::createEntity(ENTITY_TAG);
  entity->box.size = SIZE;
  entity->box.setWithCenter(centerPosition);
  configureInstance(entity);
}}

void {}::configureInstance(Entity* entity) {{
  if ((std::string)IMAGE_FILE != "") {{
    Sprite *sprite = entity->add<Sprite>();
    sprite->image = {{IMAGE_FILE}};
  }}

  entity->add<{}>();
}}
"""

NEW_MODER_TEMPLATE = """#pragma once

#include "Charlie2D.hpp"

void modify{}(Entity* entity);

"""
NEW_MODER_TEMPLATE_CPP = """#include "{}_modifer.hpp"

void modify{}(Entity* entity) {{
}}
"""

def create_component_file(name: str):
    name = name.replace(' ', '')

    components_path = os.path.join(SRC_DIRECTORY, "components")
    if not os.path.exists(components_path):
        os.makedirs(components_path)

    with open(os.path.join(components_path, f"{name}_component.hpp"), "w+") as f:
        f.write(NEW_COMPONENT_TEMPLATE.format(name))

    with open(os.path.join(components_path, f"{name}_component.cpp"), "w+") as f:
        f.write(NEW_COMPONENT_TEMPLATE_CPP.format(name, name, name))

def create_creater_file(name: str):
    name = name.replace(' ', '')

    creaters_path = os.path.join(SRC_DIRECTORY, "creaters")
    if not os.path.exists(creaters_path):
        os.makedirs(creaters_path)

    with open(os.path.join(creaters_path, f"{name}_mce.hpp"), "w+") as f:
        f.write(NEW_CREATER_TEMPLATE.format(name))

    with open(os.path.join(creaters_path, f"{name}_mce.cpp"), "w+") as f:
        f.write(NEW_CREATER_TEMPLATE_CPP.format(name, name, name, name, name, name))

def create_moder_file(name: str):
    name = name.replace(' ', '')

    moders_path = os.path.join(SRC_DIRECTORY, "modifiers")
    if not os.path.exists(moders_path):
        os.makedirs(moders_path)

    with open(os.path.join(moders_path, f"{name}_modifer.hpp"), "w+") as f:
        f.write(NEW_MODER_TEMPLATE.format(name.capitalize()))

    with open(os.path.join(moders_path, f"{name}_modifer.cpp"), "w+") as f:
        f.write(NEW_MODER_TEMPLATE_CPP.format(name, name.capitalize(), name))

def main():
    if (sys.argv[1] == 'new_creater'):
        create_creater_file(sys.argv[2])
    elif (sys.argv[1] == 'new_component'):
        create_component_file(sys.argv[2])
    elif (sys.argv[1] == 'new_moder'):
        create_moder_file(sys.argv[2])
    elif (sys.argv[1].lower() == 'help'):
        print(HELP_DISPLAY)
    else:
        print("Not a valid command")

if __name__ == '__main__':
    main()

