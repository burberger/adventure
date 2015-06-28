#include "item.hpp"
#include <iostream>

using namespace Game;

std::string Item::GetName() {
  return name;
}

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
      std::cerr << "Item " << name << " hasiInvalid value for damage!" << std::endl;
      return false;
    }
  }
  return true;
}
