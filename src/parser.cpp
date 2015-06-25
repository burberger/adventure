/**
 * Parser functions
 * Bob Urberger
 * 6/21/2015
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "parser.hpp"

using namespace Game;

/**
 * Inserters for key-value stores that make up the parsing grammar
 */
void Parser::AddWord(const std::string & word, const std::string & grammar) {
  dict.Insert(word, grammar);
}

void Parser::AddRule(const std::string & name,
                     std::vector<std::string>* const rule) {
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
    Token t = { word, dict.Find(word) };
    buf.push_back(t);
  }
  return buf;
}

/**
 * Checks the grammar of a parsed line against the rule set and returns
 * the matched rule if present
 */
std::string Parser::MatchRule(std::vector<Token> parsedLine) {
  //Check if rule exists for first token in line
  std::string ruleName = parsedLine.front().grammar;
  auto rule = rules.Find(ruleName);
  if (!rule) {
    return "";
  }

  //Start tokenIter past rule determining token
  auto tokenIter = parsedLine.begin()+1;
  auto ruleIter = rule->begin();
  //compare line grammar and rule grammar, return if mismatch
  while (ruleIter != rule->end() and tokenIter != parsedLine.end()) {
    //Identify optional grammar flag and remove for matching
    std::string currentRule = *ruleIter;
    bool optionalGrammar = (currentRule.front() == '*');
    if (optionalGrammar) {
      currentRule = currentRule.substr(1);
    }

    //Check for direct match
    if (currentRule == tokenIter->grammar) {
      ruleIter++;
      tokenIter++;
    //Filter out optional grammar
    } else if (optionalGrammar) {
      ruleIter++;
    //Return empty on required grammar failure
    } else {
      return "";
    }
  }

  //Only return rule match if we matched the entire line grammar
  if (tokenIter == parsedLine.end()) {
    return ruleName;
  }
  return "";
}
