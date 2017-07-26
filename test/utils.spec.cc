#include <iostream>
#include <assert.h>
#include "../src/utils/index.h"
#include "../src/utils/binary-tree.h"
#include "../src/utils/symbol.h"

using namespace std;

static int __TIGER_UNIT_TEST = describe("utils", [] {

	it("String constructor should be correct", [] {
		auto tpl = "XML";
		auto s = String(tpl);
		assert(strcmp(s, tpl) == 0);
	});

	it("alloc memory should be correct", [] {
		free(checkedMalloc(1024));
	});

	it("streq shoud compare two strings correct", [] {
		assert(streq("1234", "1234"));
		assert(streq("1234", "12344") == false);
	});

    it("Symbol compare should be correct", [] {
        auto a = Symbol4("a");
        auto a1 = Symbol4("a");
        auto b = Symbol4("b");
        auto A = Symbol4("A");
        auto ab = Symbol4("ab");
        assert(S_greaterthan(a, a1) == false);
        assert(S_greaterthan(a, b) == false);
        assert(S_greaterthan(a, A));
        assert(S_greaterthan(ab, a));
        assert(S_greaterthan(a, ab) == false);
    });

	it("binary tree should insert correct", [] {
		auto A = -1;
		auto a = 1;
		auto b = 2;
		auto c = 3;
		auto s_A = Symbol4("A");
		auto s_a = Symbol4("a");
		auto s_b = Symbol4("b");
		auto s_c = Symbol4("c");
		
		auto tree = new BinaryTree();
        assert(tree->root == nullptr);

		auto status = tree->insert(s_a, &a);
		auto root = tree->root;
        assert(status == 0);
		assert(root->key == s_a);
		assert(root->value == &a);
		assert(root->left == nullptr);
		assert(root->right == nullptr);

		status = tree->insert(s_b, &b);
		auto left = root->left;
		assert(status == 0);
		assert(left->key == s_b);
		assert(left->value == &b);
		assert(left->left == nullptr);
		assert(left->right == nullptr);

		status = tree->insert(s_A, &A);
		auto right = root->right;
		assert(status == 0);
		assert(right->key == s_A);
		assert(right->value == &A);
		assert(right->left == nullptr);
		assert(right->right == nullptr);

		status = tree->insert(s_c, &c);
		auto grandson = left->left;
		assert(status == 0);
		assert(grandson->key == s_c);
		assert(grandson->value == &c);
		assert(grandson->left == nullptr);
		assert(grandson->right == nullptr);

		status = tree->insert(s_a, &a);
		assert(status == 1);
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
        auto copy = tree->insertImmutable(s_a, &a);
        auto root = copy->root;
        assert(tree->root == nullptr);
        assert(copy != tree);
        assert(root);
        assert(root->key == s_a);
        assert(root->value == &a);
        assert(root->left == nullptr);
        assert(root->right == nullptr);

        tree->insert(s_a, &a);
        tree->insert(s_b, &b);

		copy = tree->insertImmutable(s_A, &A);
        root = copy->root;
		auto right = root->right;
        assert(copy);
		assert(copy != tree);
		assert(right->key == s_A);
		assert(right->value == &A);
		assert(right->left == nullptr);
		assert(right->right == nullptr);
        assert(root->left == tree->root->left);

        auto copy2 = copy->insertImmutable(s_c, &c);
        root = copy2->root;
        auto left = root->left->left;
        assert(copy2);
        assert(copy2 != copy);
        assert(left->key == s_c);
        assert(left->value == &c);
        assert(left->left == nullptr);
        assert(left->right == nullptr);
        assert(root->left != copy->root->left);
        assert(root->right == copy->root->right);

        auto copy3 = copy2->insertImmutable(s_AT, &AT);
        root = copy3->root;
        right = root->right->right;
        assert(copy3);
        assert(copy3 != copy2);
        assert(right->key == s_AT);
        assert(right->value == &AT);
        assert(right->left == nullptr);
        assert(right->right == nullptr);
        assert(root->right != copy2->root->right);
        assert(root->left == copy2->root->left);
	});
});
