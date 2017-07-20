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

	it("binary tree should insert correct", [] {
		auto A = -1;
		auto a = 1;
		auto b = 2;
		auto c = 3;
		auto s_A = Symbol4("A");
		auto s_a = Symbol4("a");
		auto s_b = Symbol4("b");
		auto s_c = Symbol4("c");
		
		auto root = new BinaryTree(s_a, &a);
		assert(root.getKey() == s_a);
		assert(root.getValue() == &a);
		assert(root.getLeft() == nullptr);
		assert(root.getRight == nullptr);

		auto status = root.insert(s_b, &b);
		auto left = root.getLeft();
		assert(status == 0);
		assert(left.getKey() == s_b);
		assert(left.getValue() == &b);
		assert(left.getLeft() == nullptr);
		assert(left.getRight() == nullptr);

		status = root.insert(s_A, &A);
		auto right = root.getRight();
		assert(status == 0);
		assert(right.getKey() == s_A);
		assert(right.getValue() == &A);
		assert(right.getLeft() == nullptr);
		assert(right.getRight() == nullptr);
		
		status = root.insert(s_c, &c);
		auto grandson = left.getLeft();
		assert(status == 0);
		assert(grandson.getKey() == s_c);
		assert(grandson.getValue() == &c);
		assert(grandson.getLeft() == nullptr);
		assert(grandson.getRight() == nullptr);

		status = root.insert(s_a, &a);
		assert(status == 1);
	});

	it("binary tree should insert immutably correct", [] {
		auto A = -1;
		auto a = 1;
		auto b = 2;
		auto c = 3;
		auto s_A = Symbol4("A");
		auto s_a = Symbol4("a");
		auto s_b = Symbol4("b");
		auto s_c = Symbol4("c");

		auto root = new BinaryTree(s_a, &a);
		root.insert(s_A, &A);
		root.insert(s_c, &c);
		auto copy = root.insertImmutable(s_b, &b);
		auto left = copy.getLeft();
		assert(root.getLeft() == nullptr);
		assert(copy != root);
		assert(copy.getKey() == s_a);
		assert(copy.getValue() == &a);
		assert(left.getKey() == s_b);
		assert(left.getValue() == &b);
	})
});
