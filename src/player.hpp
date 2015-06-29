/**
 * Bob Urberger
 * Defines class for maintaining state of the player
 */
#include "item.hpp"
#include "room.hpp"
#include <vector>

#ifndef GAME_PLAYER_HPP
#define GAME_PLAYER_HPP

namespace Game {

class Player {
  Room* currentRoom;

  Trie<Item*> inventory;

  bool alive;

  int health;

  public:
    Player() : currentRoom(NULL), inventory(), alive(true), health(100) {};

    void Move(Room* room);

    std::string UseItem(std::string itemName);

    void AddInventory(std::string name, Game::Item* item);

    void DelInventory(std::string name);

    void PrintInventory();

    bool IsAlive();

    Game::Room* GetRoom();

    int DamagePlayer(int dmg);
};

} // end namespace Game

#endif

