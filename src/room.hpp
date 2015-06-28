/**
 * Bob Urberger
 * 6/25/2015
 * Describes a room class for the game dungeon
 */

#include "../rapidjson/document.h"
#include "trie.hpp"
#include "item.hpp"
#include "parser.hpp"
#include <string>
#include <vector>
#include <iostream>

#ifndef GAME_ROOM_HPP
#define GAME_ROOM_HPP

namespace Game {

class Room {
   Trie<Room*> neighbors;

   Trie<Item*> items;

  rapidjson::Value actions;

  std::string description;

  Trie<int> state;

  public:
    bool ParseRoom(std::string name, rapidjson::Value & roomObj, Trie<Game::Item*> itemTable);

    void AddNeighbor(std::string direction, Room* room);

    Room* GetNeighbor(std::string direction);

    Game::Item* TakeItem(std::string item);

    void AddItem(std::string name, Game::Item* item);

    std::string DoAction(std::string action, std::vector<Game::Token> line);

    std::string GetDescription();
};

} // end namespace Game

#endif

