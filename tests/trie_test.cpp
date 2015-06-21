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
  t.Insert("look at", "action");
  t.Insert("pick up", "action");
  t.Insert("go", "movement");
  BOOST_CHECK(t.Find("look at") == "action");
  BOOST_CHECK(t.Find("look") == "action");
  BOOST_CHECK(t.Find("looker") == "");
  BOOST_CHECK(t.Find("look at rock") == "");
  t.Insert("look", "action");
  BOOST_CHECK(t.Find("go") == "movement");
  BOOST_CHECK(t.Find("pick") == "action");
  BOOST_CHECK(t.Find("pick up") == "action");
}

BOOST_AUTO_TEST_CASE(insertAndDelete) {
  Game::Trie t;
  t.Insert("north", "direction");
  BOOST_CHECK(t.Find("north") == "direction");
  t.Insert("next", "action");
  t.Delete("north");
  BOOST_CHECK(t.Find("north") == "");
  BOOST_CHECK(t.Find("next") == "action");
  t.Delete("next");
  BOOST_CHECK(t.Find("next") == "");
  t.Insert("next", "direction");
  BOOST_CHECK(t.Find("next") == "direction");
}

BOOST_AUTO_TEST_SUITE_END()

