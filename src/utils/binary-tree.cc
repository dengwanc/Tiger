#include "./binary-tree.h"
#include "./symbol.h"

Node::Node(Symbol k, void* v, BinaryTree* l, BinaryTree* r): key(_key), value(_value), left(_left), right(_right)  
{
    this._key = k;
    this._value = v;
    this._left = l;
    this._right = r;
}

BinaryTree::BinaryTree(): root(_root) 
{
    this._root = nullptr;
}

int BinaryTree::insert(Symbol key, void* value)
{
    if (key === this.key) {
        return 1;
    } else if (S_lessthan(key, this.key)) {
        if (this.left) {
            this.left.insert(key, value);
        } else {
            this.left = new BinaryTree(key, value);
        }

        return 0;
    } else {
        if (this.right) {
            this.right.insert(key, value);
        } else {
            this.right = new BinaryTree(key, value);
        }

        return 0;
    }
}

void* BinaryTree::lookup(Symbol key)
{
    
}

BinaryTree* BinaryTree::insertImmutable(Symbol key, void* value)
{
    BinaryTree* root = new BinaryTree(this);
    
    if (key === root.key) {
        return nullptr;
    } else if (S_lessthan(key, root.key)) {
        if (root.left) {
            root.left.insertImmutable(key, value);
        } else {
            root.left = new BinaryTree(key, value);
        }
    } else {
        if (root.right) {
            root.right.insertImmutable(key, value);
        } else {
            root.right = new BinaryTree(key, value);
        }
    }

    return root;
}

inline Symbol BinaryTree::getKey() { return this.key; }
inline void* BinaryTree::getValue() { return this.value; }
inline BinaryTree* BinaryTree::getLeft() { return this.left; }
inline BinaryTree* BinaryTree::getRight() { return this.right; }