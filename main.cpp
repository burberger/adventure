#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "src/parser.hpp"
#include <iostream>
#include <cstdio>

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
  Game::Trie<int> StateTable;
  std::vector<Game::Token> words;
  parser.LoadConfig(config);

  if (config.HasMember("title")) {
    std::cout << config["title"].GetString() << std::endl;
  }
 
  // Game loop
  while (true) {
    std::string input = readline("> ");
    if (input == "") {
      continue;
    }

    add_history(input.c_str());

    parser.ParseLine(input, words);
    std::string match = parser.MatchRule(words);
    for (auto w : words) {
      std::cout << w.word << ": " << w.grammar << std::endl;
    }
    std::cout << "Rule: " << match << std::endl;
    words.clear();
  }
  std::cout << "Game Over" << std::endl;
  return 0;
}
