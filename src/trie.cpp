/**
 * Trie Functions
 * Bob Urberger
 * 6/20/2015
 */
#include "trie.h"
#include <iostream>

using namespace Game;

/**
 * Node Constructors
 */

Node::Node() :
  element('\0'), typeclass(""), child(NULL), next_node(NULL) {};

Node::Node(char elem, std::string tc) :
  element(elem), typeclass(tc), child(NULL), next_node(NULL) {};

/**
 * Add a child node to this node
 * Child nodes are stored in a linked list, with the head of the list always
 * pointed to by child. Neighbors are stored in next_node.
 */
Node* Node::AddChild(char elem, std::string tc) {
  //No children exist, start a new list
  if (child == NULL) {
    child = new Node(elem, tc);
    return child;
  }
  //Child list exists, find the end of the list and add node
  Node* temp = child;
  while (temp->next_node) {
    temp = temp->next_node;
  }
  temp->next_node = new Node(elem, tc);
  return temp->next_node;
}

/**
 * Removes a child from the neighbor node linked list if it has no children
 */
void Node::DelChild(char elem) {
  if (child == NULL) {
    return;
  }
  Node* temp = child;
  Node* prev = temp;
  //Find element in linked list
  while (temp->element != elem) {
    prev = temp;
    temp = temp->next_node;
    if (temp == NULL) {
      return;
    }
  }
  //Only delete if leaf node
  if (temp->child == NULL) {
    //Reassign start of list if at beginning
    if (temp == child) {
      child = child->next_node;
    } else {
      prev->next_node = temp->next_node;
    }
    //Remove pointer into list and delete node
    temp->next_node = NULL;
    delete temp;
  }
}

/**
 * Find a child node containing the search character
 */
Node* Node::FindChild(char elem) {
  if (child == NULL) {
    return NULL;
  }
  Node* temp = child;
  while (temp and temp->element != elem) {
    temp = temp->next_node;
  }
  return temp;
}

std::string Node::GetTypeclass() {
  return typeclass;
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
 * Inserts a string into the set
 */
void Trie::Insert(std::string key, std::string typeclass) {
  Node* current_node = root;
  for (auto c : key) {
    Node* match = current_node->FindChild(c);
    if (match) {
      current_node = match;
    } else {
      current_node = current_node->AddChild(c, "");
    }
  }
  current_node->AddChild('\0', typeclass);
}

/**
 * Recursively deletes a key from the trie
 * Finds the path to the leaf, then deletes children backwards up the tree
 */
void Trie::RecursiveDel(std::string key, Node* current_node) {
  Node* next_node = current_node->FindChild(key[0]);
  if (key != "" and next_node != NULL) {
    RecursiveDel(key.substr(1), next_node);
  }
  current_node->DelChild(key[0]);
}

void Trie::Delete(std::string key) {
  RecursiveDel(key, root);
}

Node* Trie::Find(std::string key) {
  return Find(key, root);
}

/**
 * Finds a string within the set, returns the typeclass of a match
 */
Node* Trie::Find(std::string key, Node* head) {
  Node* current_node = head;
  Node* match;
  for (auto c : key) {
    match = current_node->FindChild(c);
    if (!match) {
      return current_node;
    }
    //Child exists, continue taversing tree
    current_node = match;
  }
  //Return the match terminator if found, otherwise return this node
  current_node = match->FindChild('\0');
  if (current_node) {
    return current_node;
  }
  return match;
}

Trie::~Trie() {
  delete root;
}

