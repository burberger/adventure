#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../src/trie.hpp"

using namespace std;

/**
 * Unit tests for node class
 */
BOOST_AUTO_TEST_SUITE(trie_node)

BOOST_AUTO_TEST_CASE(findChild) {
  Game::Node<string> a('a', "a");
  a.AddChild('b', "b");
  BOOST_CHECK(a.FindChild('b') != NULL);
  BOOST_CHECK(a.FindChild('c') == NULL);
}

BOOST_AUTO_TEST_CASE(deleteChild) {
  Game::Node<string> a('a', "a");
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
BOOST_AUTO_TEST_SUITE(trie)

BOOST_AUTO_TEST_CASE(insertAndFind) {
  Game::Trie<string> t;
  t.Insert("look", "action");
  t.Insert("at", "fluff");
  t.Insert("pick", "action");
  t.Insert("go", "movement");
  t.Insert("up", "fluff");
  t.Insert("pick", "action");
  BOOST_CHECK(t.Find("at") == "fluff");
  BOOST_CHECK(t.Find("look at") == "");
  BOOST_CHECK(t.Find("look") == "action");
  BOOST_CHECK(t.Find("looker") == "");
  BOOST_CHECK(t.Find("look at rock") == "");
  t.Insert("look", "action");
  BOOST_CHECK(t.Find("go") == "movement");
  BOOST_CHECK(t.Find("up") == "fluff");
  BOOST_CHECK(t.Find("pick up") == "");
}

BOOST_AUTO_TEST_CASE(insertAndDelete) {
  Game::Trie<string> t;
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

BOOST_AUTO_TEST_CASE(bracketOperator) {
  Game::Trie<int> t;
  t["hello"] = 5;
  t["world"] = 7;
  BOOST_CHECK(t["hello"] == 5);
  BOOST_CHECK(t["world"] == 7);
  BOOST_CHECK(t["dne"] == 0);
  int &a = t["test"];
  a = 251;
  BOOST_CHECK(t["test"] == 251);
}

BOOST_AUTO_TEST_SUITE_END()

