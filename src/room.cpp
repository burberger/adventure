/**
 * Bob Urberger
 * 6/25/15
 * Implementation for room class
 */
#include "room.hpp"
#include <vector>

using namespace Game;

/**
 * Builds all class state on the room from the DOM object representing it
 * Fetches items from the dungeon item table and adds them to the rooms inventory
 * Registers per room state and actions
 */
bool Room::ParseRoom(std::string name, rapidjson::Value & roomObj, Trie<Game::Item*> & itemTable) {
  if (roomObj.HasMember("description")) {
    description = roomObj["description"].GetString();
  } else {
    std::cerr << "Room " << name << " has no description!" << std::endl;
    return false;
  }

  if (roomObj.HasMember("inspect")) {
    inspect = roomObj["inspect"];
    if (!inspect.IsObject() and !inspect.IsString()) {
      std::cerr << "Room " << name << " has invlid inspect type!" << std::endl;
      return false;
    }
  }

  if (roomObj.HasMember("items")) {
    rapidjson::Value& itemsObj = roomObj["items"];
    if (!itemsObj.IsArray()) {
      std::cerr << "Room " << name << " items is not an array!" << std::endl;
      return false;
    }
    //Find any existing items possessed by the room in the dungeon item table
    //and copy their pointers to the local room lookup table
    for (rapidjson::SizeType i = 0; i < itemsObj.Size(); ++i) {
      std::string itemName = itemsObj[i].GetString();
      Item* foundItem = itemTable.Find(itemName);
      if (foundItem) {
        items.Insert(itemName, foundItem);
      } else {
        std::cerr << "Could not find item " << itemName << " in room " << name << std::endl;
        return false;
      }
    }
  }

  //Copy the actions behavior table from the DOM for later inspection on use
  if (roomObj.HasMember("actions")) {
    actions = roomObj["actions"];
    if (actions.IsObject()) {
      std::cerr << "Room " << name << " actions is not an object!" << std::endl;
      return false;
    }
  }

  //Load room state variables to local state registry
  if (roomObj.HasMember("state")) {
    rapidjson::Value& stateObj = roomObj["state"];
    if (!stateObj.IsObject()) {
      std::cerr << "Room " << name << " state is not an object!" << std::endl;
      return false;
    }
    for (auto itr = stateObj.MemberBegin(); itr != stateObj.MemberEnd(); ++itr) {
      if (itr->value.IsInt()) {
        state.Insert(itr->name.GetString(), itr->value.GetInt());
      } else {
        std::cerr << "Room " << name << " state " << itr->name.GetString() << " must be int!" << std::endl;
        return false;
      }
    }
  }
  return true;
}

void Room::AddNeighbor(std::string direction, Room* room) {
  neighbors[direction] = room;
}

Room* Room::GetNeighbor(std::string direction) {
  return neighbors.Find(direction);
}

/**
 * Finds an item in the rooms item table, deletes its entry from the table if
 * it's found, and returns the pointer to the item
 */
Item* Room::TakeItem(std::string item) {
  Item* itemPtr = items.Find(item);
  //Matched item, remove from table and return
  if (itemPtr) {
    items.Delete(item);
  }
  return itemPtr;
}

/**
 * Stub for room specific action implementation
 */
std::string Room::DoAction(std::string action, std::vector<Game::Token> line) {
  return action;
}

void Room::SetState(std::string key, int val) {
  state[key] = val;
}

std::string Room::GetDescription() {
  return description;
}

/**
 * Return the inspect data based on context in the DOM
 * and state within the room
 */
std::string Room::Inspect() {
  if (inspect.IsString()) {
    return inspect.GetString();
  }
  auto itr = inspect.MemberBegin();
  if (itr != inspect.MemberEnd()) {
    int stateVal = state.Find(itr->name.GetString());
    if (unsigned(stateVal) < itr->value.Size()) {
      return itr->value[stateVal].GetString();
    } else {
      return "Runtime Error: State out of range";
    }
  }
  return "Runtime Error: invalid state";
}

