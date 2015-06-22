/**
 * Trie
 * Bob Urberger
 * 6/20/2015
 * Defines a trie class
 * Stores arbitrary strings for lookup, allows for the identification
 * of the typeclass of a matched string.
 * Typeclasses of matches provide grammar for the parsing action
 */

#include <string>

namespace Game {
  class Node {
    //Character stored in this node of the trie
    char element;
    //Type of parsing element stored
    std::string typeclass;
    //Child linked list from this node
    Node* child;
    //Neighbor node in linked list
    Node* next_node;

    public:
      Node();

      Node(char elem, std::string tc);

      Node* AddChild(char elem, std::string tc);

      void DelChild(char elem);

      Node* FindChild(char elem);

      std::string GetTypeclass();

      ~Node();
  };

  class Trie {
      Node* root;

      void RecursiveDel(std::string key, Node* current_node);

    public:
      Trie();

      void Insert(std::string key, std::string typeclass);

      void Delete(std::string key);

      Node* Find(std::string key);

      Node* Find(std::string key, Node* head);

      ~Trie();
  };
}

