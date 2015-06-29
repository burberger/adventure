#include "dungeon.hpp"
#include <iostream>

using namespace Game;

/**
 * Builds the item table and lookup trie for the dungeon
 * Item table must be built before building the room graph
 */
bool Dungeon::loadItems(rapidjson::Document & config, Trie<Item*> & items, Parser & parser) {
  if (!config.HasMember("items")) {
    return false;
  }

  //Loop over item set and build the item table
  rapidjson::Value& itemsObj = config["items"];
  for (auto itr = itemsObj.MemberBegin(); itr != itemsObj.MemberEnd(); ++itr) {
    //Error out on formatting
    if (!itr->value.IsObject()) {
      std::cerr << "Could not parse " << itr->name.GetString();
      std::cerr << ": must be json object." << std::endl;
      return false;
    }
    //Build item and parse object
    Item* item = new Item;
    std::string name = itr->name.GetString();
    if (item->ParseItem(name, itr->value)) {
      itemTable.push_back(item);
      items.Insert(name, item);
      parser.AddWord(name, "item");
    } else {
      delete item;
      return false;
    }
  }
  return true;
}

/**
 * Same procedure as the item parser, with modifications for item registeration and
 * connecting the rooms after they've been parsed
 */
bool Dungeon::loadRooms(rapidjson::Document & config, Trie<Room*> & rooms, Trie<Item*> & items) {
  if (!config.HasMember("dungeon")) {
    std::cerr << "No dungon present!" << std::endl;
    return false;
  }

  rapidjson::Value& roomsObj = config["dungeon"];
  for (auto itr = roomsObj.MemberBegin(); itr != roomsObj.MemberEnd(); ++itr) {
    //Error out on formatting
    if (!itr->value.IsObject()) {
      std::cerr << "Could not parse " << itr->name.GetString();
      std::cerr << ": must be json object." << std::endl;
      return false;
    }
    //Build room and parse object
    Room* room = new Room;
    std::string name = itr->name.GetString();
    if (room->ParseRoom(name, itr->value, items)) {
      roomTable.push_back(room);
      rooms.Insert(name, room);
    } else {
      delete room;
      return false;
    }
  }

  //Second pass for building room connectivity graph
  //Loop over each room in the DOM, and then iterate through the connectivity data and
  //load room pointers as discovered
  for (auto itr = roomsObj.MemberBegin(); itr != roomsObj.MemberEnd(); ++itr) {
    if (!itr->value.HasMember("neighbors")) {
      std::cerr << itr->name.GetString() << " has no neighbors object!" << std::endl;
      return false;
    }
    //Iterate through neighbors key value pairs
    rapidjson::Value& neighborsObj = itr->value["neighbors"];
    for (auto neighborsItr = neighborsObj.MemberBegin(); neighborsItr != neighborsObj.MemberEnd(); ++neighborsItr) {
      if (!neighborsItr->value.IsString()) {
        std::cerr << itr->name.GetString() << " has invalid neighbors object!" << std::endl;
        return false;
      }
      //Make sure neighbor exists and add to this neighbors object
      if (!neighborsItr->value.IsNull()) {
        Room* neighbor = rooms.Find(neighborsItr->value.GetString());
        if (!neighbor) {
          std::cerr << itr->name.GetString() << " has nonexistent neighbor!" << std::endl;
          return false;
        }
        rooms[itr->name.GetString()]->AddNeighbor(neighborsItr->name.GetString(), neighbor);
      }
    }
  }
  return true;
}

bool Dungeon::LoadDungeon(rapidjson::Document & config, Parser & parser) {
  Trie<Item*> items;
  Trie<Room*> rooms;
  bool itemResult = loadItems(config, items, parser);
  bool roomResult = loadRooms(config, rooms, items);
  return itemResult and roomResult;
}
