#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "src/parser.hpp"
#include "src/dungeon.hpp"
#include "src/player.hpp"
#include <iostream>

#ifdef __APPLE__
#include <editline/readline.h>
#else
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace rapidjson;

Document parseFile(std::string filename) {
  FILE* fp = fopen(filename.c_str(), "r");
  if (!fp) {
    std::cerr << "config.json not found, exiting" << std::endl;
    exit(1);
  }
  //Create a stream buffer, process file as character stream
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  Document d;
  d.ParseStream(is);
  fclose(fp);
  //Return errors if present with character offset into file
  if (d.HasParseError()) {
    std::cerr << "Error (offset " << d.GetErrorOffset() << ") : "
      << GetParseError_En(d.GetParseError()) << std::endl;
    exit(1);
  }
  return d;
}

void printArray(std::string msg, Value & val) {
  std::cout << msg;
  for (SizeType i = 0; i < val.Size()-1; ++i) {
    std::cout << val[i].GetString() << ", ";
  }
  std::cout << val[val.Size()-1].GetString() << std::endl;
}

void printHelp(Document & config) {
  Value& words = config["words"];
  printArray("Inspect environment with: ", words["introspect"]);
  printArray("Use items with: ", words["action"]);
  printArray("Take items with: ", words["add_inventory"]);
  printArray("Drop items with: ", words["del_inventory"]);
  printArray("View inventory with: ", words["view_inventory"]);
  printArray("Move between rooms with: ", words["movement"]);
  printArray("Quit the game with: ", words["quit"]);

  std::cout << "Many of these phrases can be used with prepositions, such as 'pick up cat'." << std::endl;
  std::cout << "All words in a group have equivalent behavior, i.e. take and pick up do the same thing" << std::endl;
}

void fail() {
  std::cout << "I don't understand." << std::endl;
}

int main(int argc, char const* argv[]) {
  Document config =  parseFile("config.json");
  Game::Parser parser;
  Game::Dungeon dungeon;
  Game::Player player;
  std::vector<Game::Token> words;

  parser.LoadConfig(config);
  if (!dungeon.LoadDungeon(config, parser)) {
    std::cerr << "Failed to load dungeon from config!" << std::endl;
    return 1;
  }

  if (config.HasMember("title")) {
    std::cout << config["title"].GetString() << std::endl;
  }

  //place player in dungeon
  player.Move(dungeon.GetStart());

  // Game loop
  Game::Item* item;
  while (player.IsAlive()) {
    std::string input = readline("> ");
    if (input == "") {
      continue;
    }

    add_history(input.c_str());

    parser.ParseLine(input, words);
    std::string match = parser.MatchRule(words);

    if (match == "view_inventory") {
      player.PrintInventory();
    }

    else if (match == "add_inventory") {
      item = player.GetRoom()->TakeItem(words.back().word);
      if (item) {
        player.AddInventory(words.back().word, item);
      } else {
        fail();
      }
    }

    else if (match == "del_inventory") {
      player.DelInventory(words.back().word);
    }

    else if (match == "introspect") {
      if (words.back().grammar == "room") {
        std::cout << player.GetRoom()->Inspect() << std::endl;
        std::cout << "Door directions:" << std::endl;
        for (auto door : player.GetRoom()->ListDoors()) {
          std::cout << door << std::endl;
        }
      } else if (words.back().grammar == "item") {
        item = player.GetItem(words.back().word);
        if (item) {
          std::cout << item->GetDescription() << std::endl;
        } else {
          item = player.GetRoom()->GetItem(words.back().word);
          if (item) {
            std::cout << item->GetDescription() << std::endl;
          } else {
            fail();
          }
        }
      }
    }

    else if (match == "action") {
      std::string result = player.UseItem(words.back().word);
      if (result == "") {
        result = player.GetRoom()->UseItem(words.back().word);
        if (result == "") {
          fail();
        }
      }
      std::cout << result << std::endl;
    }

    else if (match == "movement") {
      Game::Room* room = player.GetRoom()->GetNeighbor(words.back().word);
      if (room) {
        player.Move(room);
      } else {
        fail();
      }
    }

    else if (match == "help_text") {
      printHelp(config);
    }

    else if (match == "quit") {
      break;
    }

    else {
      fail();
    }


    words.clear();
  }
  std::cout << "Game Over!" << std::endl;
  return 0;
}
