#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "../rapidjson/error/en.h"
#include "../src/parser.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

rapidjson::Document parseFile(std::string filename) {
  FILE* fp = fopen(filename.c_str(), "r");
  if (!fp) {
    std::cerr << filename << " not found" << std::endl;
    exit(1);
  }
  //Create a stream buffer, process file as character stream
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
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

BOOST_AUTO_TEST_SUITE(parser)

BOOST_AUTO_TEST_CASE(ParseLine) {
  vector<Game::Token> words;
  Game::Parser p;
  p.AddWord("hello", "first");
  p.AddWord("world", "second");

  Game::Token tok[3] = {
    {"hello", "first"},
    {"world", "second"},
    {"cats", ""}
  };

  p.ParseLine("hello world cats", words);
  for (int i = 0; i < 3; ++i) {
    BOOST_CHECK(tok[i].word == words[i].word);
    BOOST_CHECK(tok[i].grammar == words[i].grammar);
  }
}

BOOST_AUTO_TEST_CASE(MatchRule) {
  vector<Game::Token> words;
  vector<string> rule;
  rule.push_back("*fluff");
  rule.push_back("item");
  Game::Parser p;
  p.AddWord("pick", "action");
  p.AddWord("up", "fluff");
  p.AddWord("lettuce", "item");
  p.AddRule("action", &rule);
  p.ParseLine("pick up lettuce", words);
  BOOST_CHECK(p.MatchRule(words) == "action");
  //unrecognized word
  words.clear();
  p.ParseLine("pick up bacon", words);
  BOOST_CHECK(p.MatchRule(words) == "");
  //optional grammar
  words.clear();
  p.ParseLine("pick lettuce", words);
  BOOST_CHECK(p.MatchRule(words) == "action");
  //nonexistent first rule
  words.clear();
  p.ParseLine("throw lettuce at demon", words);
  BOOST_CHECK(p.MatchRule(words) == "");
}

BOOST_AUTO_TEST_CASE(LoadConfig) {
  vector<Game::Token> words;
  rapidjson::Document d = parseFile("tests/json/test_parser.json");
  Game::Parser p;
  p.LoadConfig(d);
  p.ParseLine("pick up cat", words);
  BOOST_CHECK(p.MatchRule(words) == "action");
}

BOOST_AUTO_TEST_SUITE_END()

