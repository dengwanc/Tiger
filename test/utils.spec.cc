#include <iostream>
#include <assert.h>
#include "../src/utils/index.h"
#include "../src/utils/binary-tree.h"

using namespace std;

static int __TIGER_UNIT_TEST = describe("utils", [] {

  it("String constructor should be correct", [] {
    auto tpl = "XML";
    auto s = String(tpl);
    assert(strcmp(s, tpl)==0);
  });

  it("alloc memory should be correct", [] {
    free(checkedMalloc(1024));
  });

  it("streq shoud compare two strings correct", [] {
    assert(streq("1234", "1234"));
    assert(!streq("1234", "12344"));
  });

  it("Symbol compare should be correct", [] {
    auto a = Symbol4("a");
    auto a1 = Symbol4("a");
    auto b = Symbol4("b");
    auto A = Symbol4("A");
    auto ab = Symbol4("ab");
    assert(!S_greaterthan(a, a1));
    assert(!S_greaterthan(a, b));
    assert(S_greaterthan(a, A));
    assert(S_greaterthan(ab, a));
    assert(!S_greaterthan(a, ab));
  });

  it("binary tree should insert correct", [] {
    auto A = -1;
    auto a = 1;
    auto updated_a = 123;
    auto b = 2;
    auto c = 3;
    auto s_A = Symbol4("A");
    auto s_a = Symbol4("a");
    auto s_b = Symbol4("b");
    auto s_c = Symbol4("c");

    auto tree = new BinaryTree();
    assert(tree->root==nullptr);

    tree->update(s_a, &a);
    auto root = tree->root;
    assert(root->key==s_a);
    assert(root->value==&a);
    assert(root->left==nullptr);
    assert(root->right==nullptr);

    tree->update(s_b, &b);
    auto left = root->left;
    assert(left->key==s_b);
    assert(left->value==&b);
    assert(left->left==nullptr);
    assert(left->right==nullptr);

    tree->update(s_A, &A);
    auto right = root->right;
    assert(right->key==s_A);
    assert(right->value==&A);
    assert(right->left==nullptr);
    assert(right->right==nullptr);

    tree->update(s_c, &c);
    auto grandson = left->left;
    assert(grandson->key==s_c);
    assert(grandson->value==&c);
    assert(grandson->left==nullptr);
    assert(grandson->right==nullptr);

    tree->update(s_a, &updated_a);
    root = tree->root;
    assert(root->key==s_a);
    assert(root->value==&updated_a);
    assert(left->key==s_b);
    assert(right->key==s_A);
    assert(grandson->key==s_c);
  });

  it("binary tree should insert immutably correct", [] {
    auto A = -1;
    auto a = 1;
    auto b = 2;
    auto c = 3;
    auto AT = -2;
    auto s_A = Symbol4("A");
    auto s_a = Symbol4("a");
    auto s_b = Symbol4("b");
    auto s_c = Symbol4("c");
    auto s_AT = Symbol4("@");

    auto tree = new BinaryTree();
    auto copy = tree->updateImmutable(s_a, &a);
    auto root = copy->root;
    assert(tree->root==nullptr);
    assert(copy!=tree);
    assert(root);
    assert(root->key==s_a);
    assert(root->value==&a);
    assert(root->left==nullptr);
    assert(root->right==nullptr);

    tree->update(s_a, &a);
    tree->update(s_b, &b);

    copy = tree->updateImmutable(s_A, &A);
    root = copy->root;
    auto right = root->right;
    assert(copy);
    assert(copy!=tree);
    assert(right->key==s_A);
    assert(right->value==&A);
    assert(right->left==nullptr);
    assert(right->right==nullptr);
    assert(root->left==tree->root->left);

    auto copy2 = copy->updateImmutable(s_c, &c);
    root = copy2->root;
    auto left = root->left->left;
    assert(copy2);
    assert(copy2!=copy);
    assert(left->key==s_c);
    assert(left->value==&c);
    assert(left->left==nullptr);
    assert(left->right==nullptr);
    assert(root->left!=copy->root->left);
    assert(root->right==copy->root->right);

    auto copy3 = copy2->updateImmutable(s_AT, &AT);
    root = copy3->root;
    right = root->right->right;
    assert(copy3);
    assert(copy3!=copy2);
    assert(right->key==s_AT);
    assert(right->value==&AT);
    assert(right->left==nullptr);
    assert(right->right==nullptr);
    assert(root->right!=copy2->root->right);
    assert(root->left==copy2->root->left);
  });

  it("binary tree should lookup correct", [] {
    auto A = -1;
    auto a = 1;
    auto b = 2;
    auto c = 3;
    auto s_A = Symbol4("A");
    auto s_a = Symbol4("a");
    auto s_b = Symbol4("b");
    auto s_c = Symbol4("c");
    auto s_AT = Symbol4("@");

    auto tree = new BinaryTree();
    tree->update(s_a, &a);
    tree->update(s_b, &b);
    tree->update(s_c, &c);
    tree->update(s_A, &A);

    auto finded_a = (int*)tree->lookup(s_a);
    auto finded_b = (int*)tree->lookup(s_b);
    auto finded_c = (int*)tree->lookup(s_c);
    auto finded_A = (int*)tree->lookup(s_A);
    auto nothing = (int*)tree->lookup(s_AT);
    assert(*finded_a == a);
    assert(*finded_b == 2);
    assert(*finded_c == 3);
    assert(*finded_A == -1);
    assert(nothing ==nullptr);
  });
});
