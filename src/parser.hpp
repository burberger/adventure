/**
 * Parser
 * Bob Urberger
 * 6/21/2015
 * Class for maintaining a parsing dictionary and performing parsing operations
 * Determines types for words, and checks against ordering rules
 */

#include "../rapidjson/document.h"
#include "trie.hpp"
#include <string>
#include <vector>

#ifndef GAME_PARSER_HPP
#define GAME_PARSER_HPP

namespace Game {

struct Token {
  std::string word;
  std::string grammar;
};

class Parser {
  Trie<std::string> dict;

  Trie<std::vector<std::string>*> rules;

  std::vector<std::vector<std::string>*> ruleTable;

  void printError(std::string type, std::string grammar, std::string errorMsg);

  public:
    Parser() {};

    void AddWord(const std::string & word, const std::string & grammar);

    void AddRule(const std::string & name, std::vector<std::string>* const rule);

    bool LoadConfig(rapidjson::Document & config);

    std::vector<Token>& ParseLine(std::string line, std::vector<Token> & buf);

    std::string MatchRule(std::vector<Token> parsedLine);

    ~Parser();
};

} //end namespace Game

#endif

