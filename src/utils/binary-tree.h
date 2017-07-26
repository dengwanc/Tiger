#ifndef TIGER_UTILS_BINARY_TREE
#define TIGER_UTILS_BINARY_TREE

#include "./symbol.h"

class Node {
private:
    Symbol _key; 
    void* _value;
    Node* _left;
    Node* _right;
    Node(Symbol key, void* value);
    Node(Node* &node);

public:
    Symbol const &key;
    void* const &value;
    Node* const &left;
    Node* const &right;

friend class BinaryTree;
};

class BinaryTree {
private:
    Node* _root;

public:
    Node* const &root;

    BinaryTree();
    BinaryTree(Node* &node);
    int insert(Symbol key, void* value);
    void* lookup(Symbol key);
    BinaryTree* insertImmutable(Symbol key, void* value);
};

#endif