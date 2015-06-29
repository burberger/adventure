/**
 * Bob Urberger
 * Item function implementations
 */
#include "item.hpp"
#include "room.hpp"
#include <iostream>

using namespace Game;

/**
 * Reads item behavior in from the DOM and loads internal state
 */
bool Item::ParseItem(std::string objName, rapidjson::Value & itemObj) {
  name = objName;
  //Fetch item description from the dom
  if (itemObj.HasMember("description")) {
    description = itemObj["description"].GetString();
  } else {
    std::cerr << "Item " << objName << " has no description!" << std::endl;
    return false;
  }

  //Fetch item action
  if (itemObj.HasMember("action") and itemObj["action"].IsObject()) {
    action = itemObj["action"];
  } else {
    std::cerr << "Item " << name << " has no action!" << std::endl;
    return false;
  }

  //Fetch optional damage value
  if (itemObj.HasMember("damage")) {
    const rapidjson::Value& dmg = itemObj["damage"];
    if (dmg.IsArray() and dmg.Size() == 2) {
      damage = new int[2];
      damage[0] = dmg[0].GetInt();
      damage[1] = dmg[1].GetInt();
    } else {
      //Fail to parse if damage is not configured correctly
      std::cerr << "Item " << name << " has invalid value for damage!" << std::endl;
      return false;
    }
  }
  return true;
}

std::string Item::GetName() {
  return name;
}

std::string Item::GetDescription() {
  return description;
}

std::string Item::UseItem(Room* room) {
  std::string desc;
  for (auto itr = action.MemberBegin(); itr != action.MemberEnd(); ++itr) {
    if (itr->name.GetString() != std::string("description")) {
      if (itr->value.IsInt()) {
        room->SetState(itr->name.GetString(), itr->value.GetInt());
      } else {
        std::cerr << "Error: Invalid type for state update" << std::endl;
      }
    } else {
      desc = itr->value.GetString();
    }
  }
  return desc;
}

