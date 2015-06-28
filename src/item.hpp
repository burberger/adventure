/**
 * Bob Urberger
 * 6/25/2015
 * Describes an item class for the game
 */

#include "../rapidjson/document.h"
#include "trie.hpp"
#include <string>
#include <vector>

#ifndef GAME_ITEM_HPP
#define GAME_ITEM_HPP

namespace Game {

class Item {
  std::string name;

  std::string description;

  rapidjson::Value action;

  int* damage;

  public:
    Item() : name(""), description(""), action(), damage(NULL) {};

    bool ParseItem(std::string name, rapidjson::Value & itemObj);

    std::string GetName();

    ~Item() {
      delete[] damage;
    }
};

} // end namespace Game

#endif

