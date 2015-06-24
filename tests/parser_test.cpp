#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "../src/parser.hpp"

using namespace std;

BOOST_AUTO_TEST_SUITE(parseline)

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

BOOST_AUTO_TEST_SUITE_END()

