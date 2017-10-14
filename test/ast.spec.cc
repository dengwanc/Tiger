#include <assert.h>
#include "../src/utils/index.h"
#include "../src/ast/index.h"
#include "global.h"

static int __TIGER_UNIT_TEST = describe("ast tree", [] {
  it("should parse ast correct", [] {
    for (auto i: TIGS) {
      assert(ast::parse(i.c_str())==0);
    }
  });
});
