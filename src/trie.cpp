#include "trie.h"

using namespace Game;

/**
 * Node Constructors
 */

Node::Node() :
  element('\0'), typeclass(""), child(NULL) {};

Node::Node(char elem, std::string tc) :
  element(elem), typeclass(tc), child(NULL) {};

/**
 * Equivalence operator
 */
bool Node::operator ==(Node const& n) {
  return element == n.element;
}

/**
 * Add a child node to this node
 * Child nodes are stored in a linked list, with the head of the list always
 * pointed to by child. Neighbors are stored in next_node.
 */
Node* Node::AddChild(Node n) {
  //No children exist, start a new list
  if (child == NULL) {
    child = new Node(n);
    return child;
  }
  //Child list exists, find the end of the list and add node
  Node* temp = child;
  while (temp->next_node) {
    temp = temp->next_node;
  }
  temp->next_node = new Node(n);
  return temp->next_node;
}

/**
 * Find a child node containing the search character
 */
Node* Node::FindChild(char c) {
  if (child == NULL) {
    return NULL;
  }
  Node* temp = child;
  while (temp->next_node and temp->element != c) {
    temp = temp->next_node;
  }
  return temp;
}

/**
 * Recursively free nodes in the trie
 */
Node::~Node() {
  delete child;
  delete next_node;
}

/**
 * Trie Constructor
 */
Trie::Trie() {
  root = new Node();
}

/**
 * Inserts a string into the trie
 */
void Trie::Insert(std::string key, std::string typeclass) {
  Node* current_node = root;
  for (auto c : key) {
    Node* match = current_node->FindChild(c);
    if (match) {
      current_node = match;
    } else {
      current_node = current_node->AddChild(Node(c, typeclass));
    }
  }
}
