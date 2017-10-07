#include "./binary-tree.h"
#include <assert.h>

Node::Node(Symbol k, void *v) {
  this->key = k;
  this->value = v;
  this->left = nullptr;
  this->right = nullptr;
}

Node::Node(Node *node) {
  if (!node) throw "NullptrError: argument `node` is null";

  this->key = node->key;
  this->value = node->value;
  this->left = node->left;
  this->right = node->right;
}

BinaryTree::BinaryTree() {
  this->root = nullptr;
}

BinaryTree::BinaryTree(Node *node) {
  this->root = node;
}

void BinaryTree::update(Symbol key, void *value) {
  auto current = &(this->root);

  while (*current) {
    if (key==(*current)->key) {
      break;
    } else if (S_greaterthan(key, (*current)->key)) {
      current = &((*current)->left);
    } else {
      current = &((*current)->right);
    }
  }

  *current = new Node(key, value);
}

void *BinaryTree::lookup(Symbol key) {
  assert(key); // must not-null or sth wrong

  auto curret = this->root;

  while (curret) {
    if (key == curret->key) {
      break;
    } else if (S_greaterthan(key, curret->key)) {
      curret = curret->left;
    } else {
      curret = curret->right;
    }
  }

  return curret ? curret->value : nullptr;
}

BinaryTree *BinaryTree::updateImmutable(Symbol key, void *value) {
  if (this->root==nullptr) {
    return new BinaryTree(new Node(key, value));
  }

  if (key==this->root->key) {
    this->root->value = value;
    return this;
  }

  auto current = new Node(this->root);
  auto root = current;

  while (true) {
    if (key==current->key) {
      current->value = value;
      break;
    } else if (S_greaterthan(key, current->key)) {
      if (current->left) {
        // copy & point correctly
        current->left = new Node(current->left);
        current = current->left;
      } else {
        current->left = new Node(key, value);
        break;
      }
    } else {
      if (current->right) {
        current->right = new Node(current->right);
        current = current->right;
      } else {
        current->right = new Node(key, value);
        break;
      }
    }
  }

  return new BinaryTree(root);
}