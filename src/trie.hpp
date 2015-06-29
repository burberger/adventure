/**
 * Trie
 * Bob Urberger
 * 6/20/2015
 * Defines a trie class
 * Stores arbitrary strings as a quickly indexable key-value store
 * to a templated datatype.
 */

#ifndef GAME_TRIE_HPP
#define GAME_TRIE_HPP

#include <string>
#include <vector>
#include "node.hpp"

namespace Game {

template<typename T>
class Trie {
    Node<T>* root;

    Node<T>* insertPtr(std::string key, T value);

    Node<T>* findPtr(const std::string & key);

    void recursiveDel(std::string key, Node<T>* currentNode);

    void recursiveTraverse(std::string & prefix, std::vector<std::string> & list, Node<T>* currentNode);

  public:
    Trie() {
      root = new Node<T>();
    };

    void Insert(std::string key, T value);

    void Delete(const std::string & key);

    std::vector<std::string> ListKeys();

    T& Find(const std::string & key);

    T& operator[](std::string key);

    ~Trie() {
      delete root;
    };
};

/**
 * Inserts a string into the set, returns pointer to leaf node
 */
template<typename T>
Node<T>* Trie<T>::insertPtr(std::string key, T value) {
  Node<T>* current_node = root;
  Node<T>* match;
  //Walk tree per string characters and insert children as needed
  for (auto c : key) {
    match = current_node->FindChild(c);
    if (match) {
      current_node = match;
    } else {
      current_node = current_node->AddChild(c);
    }
  }
  //Check for temination node, if not found it's a new word, add to set
  match = current_node->FindChild('\0');
  if (!match) {
    return current_node->AddChild('\0', value);
  }
  return match;
}

/**
 * Finds a string within the set, returns the typeclass of a match
 */
template<typename T>
Node<T>* Trie<T>::findPtr(const std::string & key) {
  Node<T>* current_node = root;
  Node<T>* match = root;
  for (auto c : key) {
    match = current_node->FindChild(c);
    if (!match) {
      return current_node;
    }
    //Child exists, continue taversing tree
    current_node = match;
  }
  //Return the match terminator if found, otherwise returns last node
  current_node = match->FindChild('\0');
  if (current_node) {
    return current_node;
  }
  return match;
}

/**
 * Recursively deletes a key from the trie
 * Finds the path to the leaf, then deletes children backwards up the tree
 */
template<typename T>
void Trie<T>::recursiveDel(std::string key, Node<T>* currentNode) {
  Node<T>* next_node = currentNode->FindChild(key[0]);
  if (key != "" and next_node != NULL) {
    recursiveDel(key.substr(1), next_node);
  }
  currentNode->DelChild(key[0]);
}

/**
 * Recursively traverse the trie and build a list of all keys present
 */
template<typename T>
void Trie<T>::recursiveTraverse(std::string & prefix, std::vector<std::string> & list, Node<T>* currentNode) {
  if (currentNode == NULL) {
    list.push_back(prefix);
  }
  Node<T>* traverse = currentNode;
  while (traverse) {
    prefix.push_back(traverse->element);
    recursiveTraverse(prefix, list, traverse->child);
    prefix.pop_back();
    traverse = traverse->next_node;
  }
}

/**
 * Public facing functions wrap internal behavior to prevent exposing node
 * pointers to the outside world
 */

template<typename T>
void Trie<T>::Insert(std::string key, T value) {
  insertPtr(key, value);
}

template<typename T>
T& Trie<T>::Find(const std::string & key) {
  return findPtr(key)->GetValue();
}

template<typename T>
void Trie<T>::Delete(const std::string & key) {
  recursiveDel(key, root);
}

template<typename T>
std::vector<std::string> Trie<T>::ListKeys() {
  std::string prefix;
  std::vector<std::string> words;
  recursiveTraverse(prefix, words, root->child);
  return words;
}

/**
 * Bracket operator for key find and update
 * If key doesn't exist, will create it and return a refrence to its value
 */
template<typename T>
T& Trie<T>::operator[](std::string key) {
  Node<T>* match = findPtr(key);
  if (match == root) {
    return insertPtr(key, T())->GetValue();
  }
  return match->GetValue();
}

} //end namespace Game

#endif

