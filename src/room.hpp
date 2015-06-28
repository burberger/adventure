/**
 * Bob Urberger
 * 6/25/2015
 * Describes a room class for the game dungeon
 */

#include <string>
#include <vector>
#include "trie.hpp"
#include "item.hpp"
#include "parser.hpp"

#ifndef GAME_ROOM_HPP
#define GAME_ROOM_HPP

namespace Game {

class Room {
  Room* neighbors;

  rapidjson::Value actions;

  rapidjson::Value description;

  Trie<int> state;

  public:
    Room() {
      neighbors = new Room[4];
    };

    bool ParseRoom(std::string name, rapidjson::Value & roomObj, Trie<Game::Item*> items);

    Game::Item* TakeItem(std::string item);

    std::string DoAction(std::string action, std::vector<Game::Token> line);

    std::string GetDescription();

    ~Room() {
      delete[] neighbors;
    }
};

} // end namespace Game

#endif

