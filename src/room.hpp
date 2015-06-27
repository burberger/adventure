/**
 * Bob Urberger
 * 6/25/2015
 * Describes a room class for the game dungeon
 */

#include <string>
#include <vector>
#include "trie.hpp"
#include "item.hpp"

#ifndef GAME_ROOM_HPP
#define GAME_ROOM_HPP

namespace Game {

class Room {
  Trie<Game::Item*> items;

  Trie<std::string> actions;

  public:
    Room();

    //Game::Item GetItem(std::string item);
    void TakeItem(std::string item);

    std::string DoAction();
};

} // end namespace Game

#endif
