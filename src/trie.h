/**
 * Defines a string trie class
 * Stores arbitrary strings for lookup, allows for the identification
 * of the typeclass of a matched string.
 * This provides a direct knowledge of parsing structure with each match
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

      bool operator ==(Node const& n);

      Node* AddChild(Node n);

      Node* FindChild(char c);

      ~Node();
  };

  class Trie {
      Node* root;
    public:
      Trie();
      void Insert(std::string key, std::string typeclass);
  };
}

