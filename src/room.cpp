/**
 * Bob Urberger
 * 6/25/15
 * Implementation for room class
 */
#include "room.hpp"

using namespace Game;

bool Room::ParseRoom(std::string name, rapidjson::Value & roomObj, Trie<Game::Item*> & itemTable) {
  if (roomObj.HasMember("description")) {
    description = roomObj["description"].GetString();
  } else {
    std::cerr << "Room " << name << " has no description!" << std::endl;
    return false;
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

std::string Room::GetDescription() {
  return description;
}
