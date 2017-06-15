#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "global.h"

extern int yyparse(void);

namespace syntax {
  int parse(const char* fname)
  {
      lexical::reset(fname);
    if (yyparse() == 0) {
      return 0;
    } else {
      return 1;
    }
  }
}

static int __TIGER_UNIT_TEST = describe("syntax tree", [] {

	it("should parse sentence correct", [] {
    assert(syntax::parse(TIGER_DEFAULT_INPUT) == 0);
	});
});
