/**
 * Node
 * Bob Urberger
 * 6/20/2015
 * Defines a trie class
 * Templated node class for trie
 */

#include <string>

namespace Game {

template <typename T>
class Node {
  //Character stored in this node of the trie
  char element;
  //Value stored for this key (normally empty except for leaf)
  T value;
  //Child linked list from this node
  Node<T>* child;
  //Neighbor node in linked list
  Node<T>* next_node;

  public:
    Node() : element('\0'), child(NULL), next_node(NULL) {};

    Node(char elem) : element(elem), child(NULL), next_node(NULL) {};

    Node(char elem, T val) :
      element(elem), value(val), child(NULL), next_node(NULL) {};

    Node<T>* AddChild(char elem);

    Node<T>* AddChild(char elem, T val);

    void DelChild(char elem);

    Node<T>* FindChild(char elem);

    T& GetValue();

    ~Node() {
      delete child;
      delete next_node;
    };
};

/**
 * Add a child node to this node
 * Child nodes are stored in a linked list, with the head of the list always
 * pointed to by child. Neighbors are stored in next_node.
 */
template <typename T>
Node<T>* Node<T>::AddChild(char elem, T val) {
  //No children exist, start a new list
  if (child == NULL) {
    child = new Node<T>(elem, val);
    return child;
  }
  //Child list exists, find the end of the list and add node
  Node<T>* temp = child;
  while (temp->next_node) {
    temp = temp->next_node;
  }
  temp->next_node = new Node<T>(elem, val);
  return temp->next_node;
}

template <typename T>
Node<T>* Node<T>::AddChild(char elem) {
  //No children exist, start a new list
  if (child == NULL) {
    child = new Node<T>(elem);
    return child;
  }
  //Child list exists, find the end of the list and add node
  Node<T>* temp = child;
  while (temp->next_node) {
    temp = temp->next_node;
  }
  temp->next_node = new Node<T>(elem);
  return temp->next_node;
}

/**
 * Removes a child from the neighbor node linked list if it has no children
 */
template <typename T>
void Node<T>::DelChild(char elem) {
  if (child == NULL) {
    return;
  }
  Node<T>* temp = child;
  Node<T>* prev = temp;
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
template <typename T>
Node<T>* Node<T>::FindChild(char elem) {
  if (child == NULL) {
    return NULL;
  }
  Node<T>* temp = child;
  while (temp and temp->element != elem) {
    temp = temp->next_node;
  }
  return temp;
}

template <typename T>
T& Node<T>::GetValue() {
  return value;
}

} //end namespace Game

