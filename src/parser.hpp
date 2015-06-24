/**
 * Parser
 * Bob Urberger
 * 6/21/2015
 * Class for maintaining a parsing dictionary and performing parsing operations
 * Determines types for words, and checks against ordering rules
 */

#include <string>
#include <vector>
#include "trie.hpp"

namespace Game {

struct Token {
  std::string word;
  std::string grammar;
};

class Parser {
  Trie<std::string> dict;

  Trie<std::string*> rules;

  public:
    Parser() {};

    void AddWord(const std::string & word, const std::string & grammar);

    void AddRule(const std::string & name, std::string* const rule);

    std::vector<Token>& ParseLine(std::string line, std::vector<Token> & buf);

    std::string MatchRule(std::vector<Token> parsedLine);

    ~Parser() {};
};

} //end namespace Game
