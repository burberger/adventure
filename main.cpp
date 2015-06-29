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

    if (match == "introspect") {
      if (words.back().grammar == "room") {
        std::cout << player.GetRoom()->Inspect() << std::endl;
      }
    }

    words.clear();
  }
  std::cout << "Game Over!" << std::endl;
  return 0;
}
