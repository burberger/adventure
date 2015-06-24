/**
 * Trie
 * Bob Urberger
 * 6/20/2015
 * Defines a trie class
 * Stores arbitrary strings as a quickly indexable key-value store
 * to a templated datatype.
 */

#include <string>
#include "node.hpp"

namespace Game {

template<typename T>
class Trie {
    Node<T>* root;

    Node<T>* insertPtr(const std::string & key, const T & value);

    Node<T>* findPtr(const std::string & key);

    void recursiveDel(std::string key, Node<T>* current_node);

  public:
    Trie() {
      root = new Node<T>();
    };

    void Insert(const std::string & key, const T & value);

    void Delete(const std::string & key);

    T& Find(const std::string & key);

    T& operator[](const std::string & key);

    ~Trie() {
      delete root;
    };
};

/**
 * Inserts a string into the set, returns pointer to leaf node
 */
template<typename T>
Node<T>* Trie<T>::insertPtr(const std::string & key, const T & value) {
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
  Node<T>* match;
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
void Trie<T>::recursiveDel(std::string key, Node<T>* current_node) {
  Node<T>* next_node = current_node->FindChild(key[0]);
  if (key != "" and next_node != NULL) {
    recursiveDel(key.substr(1), next_node);
  }
  current_node->DelChild(key[0]);
}


/**
 * Public facing functions wrap internal behavior to prevent exposing node
 * pointers to the outside world
 */

template<typename T>
void Trie<T>::Insert(const std::string & key, const T & value) {
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

/**
 * Bracket operator for key find and update
 * If key doesn't exist, will create it and return a refrence to its value
 */
template<typename T>
T& Trie<T>::operator[](const std::string & key) {
  Node<T>* match = findPtr(key);
  if (match == root) {
    return insertPtr(key, T())->GetValue();
  }
  return match->GetValue();
}

} //namespace Game
