#ifndef TIGER_UTILS_BINARY_TREE
#define TIGER_UTILS_BINARY_TREE

#include "./symbol.h"

class Node {
private:
    Symbol _key; 
    void* _value;
    BinaryTree* _left;
    BinaryTree* _right;
    Node(Symbol key, void* value, BinaryTree* left, BinaryTree* right);

public:
    Symbol const &key;
    void* const &value;
    BinaryTree* const &left;
    BinaryTree* const &right;

friend class BinaryTree;
};

class BinaryTree {
private:
    Node* _root;

public:
    Node* const &root;
    BinaryTree();
    int insert(Symbol key, void* value);
    void* lookup(Symbol key);
    BinaryTree* insertImmutable(Symbol key, void* key);
};

#endif