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
  rapidjson::Value& itemsObj = config["items"];
  //Loop over item set and build the item table
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
  if (!config.HasMember("rooms")) {
    return false;
  }

  rapidjson::Value& roomsObj = config["rooms"];
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
  return true;
}

bool Dungeon::LoadDungeon(rapidjson::Document & config, Parser & parser) {
  Trie<Item*> items;
  Trie<Room*> rooms;
  return loadItems(config, items, parser);
}