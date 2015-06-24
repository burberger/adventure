/**
 * Parser functions
 * Bob Urberger
 * 6/21/2015
 */

#include <string>
#include <sstream>
#include <vector>
#include "parser.hpp"

using namespace Game;

/**
 * Adders for key-value stores that make up the parsing grammar
 */
void Parser::AddWord(const std::string & word, const std::string & grammar) {
  dict.Insert(word, grammar);
}

void Parser::AddRule(const std::string & name, std::string* const rule) {
  rules.Insert(name, rule);
}

/**
 * Tokenizes a string and finds the grammar for each token from the dictionary
 * Returns a vector containing the token list for rule matching
 */
std::vector<Token>& Parser::ParseLine(std::string line, std::vector<Token> & buf) {
  std::stringstream stream(line);
  std::string word;
  while (std::getline(stream, word, ' ')) {
    Token t;
    t.word = word;
    t.grammar = dict.Find(word);
    buf.push_back(t);
  }
  return buf;
}

std::string Parser::MatchRule(std::vector<Token> parsedLine) {
  std::string* rule = rules.Find(parsedLine.front().grammar);
  if (!rule) {
    return "";
  }

  for (int i = 1; i < parsedLine.size(); ++i) {
    
  }
}
