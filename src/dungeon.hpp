#include "../rapidjson/document.h"
#include "parser.hpp"
#include "room.hpp"
#include "item.hpp"
#include <string>
#include <vector>
#include <iostream>

#ifndef GAME_DUNGEON_HPP
#define GAME_DUNGEON_HPP

namespace Game {

class Dungeon {
  std::vector<Game::Room*> roomTable;

  std::vector<Game::Item*> itemTable;

  bool loadItems(rapidjson::Document & config, Trie<Game::Item*> & items,
      Game::Parser & parser);

  bool loadRooms(rapidjson::Document & config, Trie<Game::Room*> & rooms,
      Trie<Game::Item*> & items);

  bool connectRooms(Trie<Game::Room*> & rooms);

  public:
    Dungeon() {};

    bool LoadDungeon(rapidjson::Document & config, Game::Parser & parser);

    void PrintItemTable() {
      for (auto item : itemTable) {
        std::cout << item->GetName() << std::endl;
      }
    }

    void PrintRoomTable() {
      for (auto room : roomTable) {
        std::cout << room->GetDescription() << std::endl;
      }
    }

    ~Dungeon() {
      for (auto r : roomTable) {
        delete r;
      }
      for (auto i : itemTable) {
        delete i;
      }
    };
};

} //end namespace Game

#endif

