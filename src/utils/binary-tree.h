#ifndef TIGER_UTILS_BINARY_TREE
#define TIGER_UTILS_BINARY_TREE

#include "./symbol.h"

class Node {
public:
  Node(Symbol key, void *value);
  Node(Node *node);

  Symbol key;
  void *value;
  Node *left;
  Node *right;

  friend class BinaryTree;
};

class BinaryTree {
public:
  Node *root;

  BinaryTree();
  BinaryTree(Node *node);
  void update(Symbol key, void *value);
  void *lookup(Symbol key);
  BinaryTree *updateImmutable(Symbol key, void *value);
};

#endif