#include "global.h"
#include "../src/ast/index.h"

static int __TIGER_UNIT_TEST = describe("ast tree", [] {
  it("should parse ast correct", [] {
    for (auto i: TIGS) {
      assert(ast::parse(i.c_str())==0);
    }
  });
});
