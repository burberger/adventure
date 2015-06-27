/**
 * Parser functions
 * Bob Urberger
 * 6/21/2015
 */

#include "../rapidjson/document.h"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace Game;

void Parser::printError(std::string type, std::string grammar,
    std::string errorMsg)
{
  std::cerr << "Error loading parsing engine:" << std::endl;
  std::cerr << type << " (" << grammar << "): " << errorMsg << std::endl;
}

/**
 * Inserters for key-value stores that make up the parsing grammar
 * These are used by the unit test system to manually load words
 */
void Parser::AddWord(const std::string & word, const std::string & grammar) {
  dict.Insert(word, grammar);
}

void Parser::AddRule(const std::string & name,
    std::vector<std::string>* const rule)
{
  rules.Insert(name, rule);
}

/**
 * Loads parser words and rules from a parsed json configuration file
 * Returns true on successful load
 */
bool Parser::LoadConfig(rapidjson::Document & config) {
  //Check for existence of required objects
  if (config.HasMember("words") and config.HasMember("grammar")) {
    rapidjson::Value& wordsObj = config["words"];
    rapidjson::Value& rulesObj = config["grammar"];

    if (!wordsObj.IsObject()) {
      printError("words", "", "words must be a json object");
      return false;
    }

    //Iterate over grammar set and update dictionary
    std::string grammar = "";
    for (auto itr = wordsObj.MemberBegin(); itr != wordsObj.MemberEnd(); ++itr) {
      grammar = itr->name.GetString();
      if (!itr->value.IsArray()) {
        printError("words", grammar, "Member value should be array");
        return false;
      }

      //Iterate over words in present grammar set and add to match pool
      for (rapidjson::SizeType i = 0; i < itr->value.Size(); ++i) {
        dict.Insert(itr->value[i].GetString(), grammar);
      }
    }

    if (!rulesObj.IsObject()) {
      printError("grammar", "", "grammar must be a json object");
      return false;
    }

    //Iterate over grammar set and add rules
    for (auto itr = rulesObj.MemberBegin(); itr != rulesObj.MemberEnd(); ++itr) {
      grammar = itr->name.GetString();
      if (!itr->value.IsArray()) {
        printError("grammar", grammar, "Member value should be array");
        return false;
      }

      //Iterate over words in present grammar set and add to rule vector
      std::vector<std::string>* rule = new std::vector<std::string>();
      for (rapidjson::SizeType i = 0; i < itr->value.Size(); ++i) {
        rule->push_back(itr->value[i].GetString());
      }
      ruleTable.push_back(rule);
      rules.Insert(grammar, rule);
    }
    //Successful build of parser data
    return true;
  }
  std::cerr << "Error: Missing or misnamed parser ruleset, please make sure you have";
  std::cerr << " a word list and a grammar set" << std::endl;
  return false;
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

Parser::~Parser() {
  for (auto rule : ruleTable) {
    delete rule;
  }
}
