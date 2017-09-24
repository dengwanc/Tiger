#include "./binary-tree.h"

Node::Node(Symbol k, void *v) : key(_key), value(_value), left(_left), right(_right) {
  this->_key = k;
  this->_value = v;
  this->_left = nullptr;
  this->_right = nullptr;
}

Node::Node(Node *&node) : key(_key), value(_value), left(_left), right(_right) {
  this->_key = node->_key;
  this->_value = node->_value;
  this->_left = node->_left;
  this->_right = node->_right;
}

BinaryTree::BinaryTree() : root(_root) {
  this->_root = nullptr;
}

BinaryTree::BinaryTree(Node *&node) : root(_root) {
  this->_root = node;
}

int BinaryTree::update(Symbol key, void *value) {
  auto current = &(this->_root);

  while (*current) {
    if (key==(*current)->key) {
      break;
    } else if (S_greaterthan(key, (*current)->key)) {
      current = &((*current)->_left);
    } else {
      current = &((*current)->_right);
    }
  }

  *current = new Node(key, value);

  return 0;
}

void *BinaryTree::lookup(Symbol key) {
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
    auto node = new Node(key, value);
    return new BinaryTree(node);
  }

  if (key==this->root->key) {
    this->root->_value = value;
    return this;
  }

  auto current = new Node(this->_root);
  auto root = current;

  while (true) {
    if (key==current->key) {
      current->_value = value;
      break;
    } else if (S_greaterthan(key, current->key)) {
      if (current->left) {
        // copy & point correctly
        current->_left = new Node(current->_left);
        current = current->_left;
      } else {
        current->_left = new Node(key, value);
        break;
      }
    } else {
      if (current->right) {
        current->_right = new Node(current->_right);
        current = current->_right;
      } else {
        current->_right = new Node(key, value);
        break;
      }
    }
  }

  return new BinaryTree(root);
}