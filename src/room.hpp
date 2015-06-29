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

  rapidjson::Value inspect;

  std::string description;

  Trie<int> state;

  public:
    bool ParseRoom(std::string name, rapidjson::Value & roomObj, Trie<Game::Item*> & itemTable);

    void AddNeighbor(std::string direction, Room* room);

    Room* GetNeighbor(std::string direction);

    Game::Item* TakeItem(std::string item);

    Game::Item* GetItem(std::string item);

    std::string UseItem(std::string item);

    std::string DoAction(std::string action, std::vector<Game::Token> line);

    void SetState(std::string key, int val);

    std::string GetDescription();

    std::string Inspect();

    std::vector<std::string> ListDoors();
};

} // end namespace Game

#endif

