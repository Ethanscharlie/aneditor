#include "Charlie2D.hpp"
#include <iostream>

#include "config.h"

/*
 * Some useful code snips
 *
 * LDTK::loadJson("res/ldtk.ldtk");
 * LDTK::onLoadLevel = [](auto entities) {
 *   for (Entity *entity : entities) {
 *     if (entity->tag == "Player")
 *      std::cout << "Player\n";
 *   }
 * };
 * LDTK::loadLevel("65ba8250-25d0-11ef-b0ef-03acbc99fa7b");
*/

int main(int, char **) {
  GameManager::init();


  GameManager::doUpdateLoop();
  return 0;
}
