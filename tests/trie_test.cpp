#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE trie_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../src/trie.h"

using namespace std;

/**
 * Unit tests for node class
 */
BOOST_AUTO_TEST_SUITE(Node)

BOOST_AUTO_TEST_CASE(findChild) {
  Game::Node a('a', "a");
  a.AddChild('b', "b");
  BOOST_CHECK(a.FindChild('b') != NULL);
  BOOST_CHECK(a.FindChild('c') == NULL);
}

BOOST_AUTO_TEST_CASE(deleteChild) {
  Game::Node a('a', "a");
  a.AddChild('b', "b");
  a.AddChild('c', "c");
  a.AddChild('d', "d");
  a.DelChild('b');
  BOOST_CHECK(a.FindChild('b') == NULL);
  a.DelChild('b');
  BOOST_CHECK(a.FindChild('c') != NULL);
  a.DelChild('d');
  BOOST_CHECK(a.FindChild('d') == NULL);
  BOOST_CHECK(a.FindChild('c') != NULL);
}

BOOST_AUTO_TEST_SUITE_END()

/**
 * Unit tests for Trie class
 */
BOOST_AUTO_TEST_SUITE(Trie)

BOOST_AUTO_TEST_CASE(insertAndFind) {
  Game::Trie t;
  t.Insert("lookat", "action");
  t.Insert("pickup", "action");
  t.Insert("go", "movement");
  Game::Node* match = t.Find("look");
  BOOST_CHECK(match->GetTypeclass() == "");
  BOOST_CHECK(t.Find("at", match)->GetTypeclass() == "action");
  BOOST_CHECK(t.Find("look at")->GetTypeclass() == "");
  BOOST_CHECK(t.Find("look")->GetTypeclass() == "");
  BOOST_CHECK(t.Find("looker")->GetTypeclass() == "");
  BOOST_CHECK(t.Find("look at rock")->GetTypeclass() == "");
  t.Insert("look", "action");
  BOOST_CHECK(t.Find("go")->GetTypeclass() == "movement");
  match = t.Find("pick");
  BOOST_CHECK(match->GetTypeclass() == "");
  BOOST_CHECK(t.Find("up", match)->GetTypeclass() == "action");
  BOOST_CHECK(t.Find("pick up")->GetTypeclass() == "");
}

BOOST_AUTO_TEST_CASE(insertAndDelete) {
  Game::Trie t;
  t.Insert("north", "direction");
  BOOST_CHECK(t.Find("north")->GetTypeclass() == "direction");
  t.Insert("next", "action");
  t.Delete("north");
  BOOST_CHECK(t.Find("north")->GetTypeclass() == "");
  BOOST_CHECK(t.Find("next")->GetTypeclass() == "action");
  t.Delete("next");
  BOOST_CHECK(t.Find("next")->GetTypeclass() == "");
  t.Insert("next", "direction");
  BOOST_CHECK(t.Find("next")->GetTypeclass() == "direction");
}

BOOST_AUTO_TEST_SUITE_END()

