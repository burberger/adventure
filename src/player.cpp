/**
 * Bob Urberger
 * Player function implementations
 */

#include "player.hpp"

using namespace Game;

void Player::Move(Room* room) {
  currentRoom = room;
  std::cout << room->GetDescription() << std::endl;
}

std::string Player::UseItem(std::string itemName) {
  Item* item = inventory.Find(itemName);
  if (item) {
    return item->Use(currentRoom);
  }
  return "";
}

Item* Player::GetItem(std::string itemName) {
  return inventory.Find(itemName);
}

void Player::AddInventory(std::string name, Item* item) {
  inventory[name] = item;
}

void Player::DelInventory(std::string name) {
  inventory.Delete(name);
}

void Player::PrintInventory() {
  auto list = inventory.ListKeys();
  std::cout << "Inventory:" << std::endl;
  if (list.size() == 1) {
    std::cout << "Empty." << std::endl;
    return;
  }
  for (auto item : list) {
    std::cout << item << std::endl;
  }
}

bool Player::IsAlive() {
  return alive;
}

Room* Player::GetRoom() {
  return currentRoom;
}

int Player::DamagePlayer(int dmg) {
  health -= dmg;
  return health;
}

