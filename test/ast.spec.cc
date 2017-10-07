#include <assert.h>
#include <stdio.h>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/ast/index.h"
#include "../src/semantic/ecode.h"
#include "global.h"

extern int yyparse(void);

namespace ast {
int parse(const char *fname) {
  lexical::reset(fname);
  if (yyparse()==0) {
    return 0;
  } else {
    return 1;
  }
}

void debug(const char *file) {
  printf("\n~~~~~~ DEBUGGING AST STARTED ~~~~~~\n\n");

  assert(ast::parse(file)==0);

  if (ast::AST_ROOT==nullptr) {
    printf("⚠️  %s has no content", TIGER_DEFAULT_INPUT);
  } else {
    ast::AST_ROOT->print();
    ast::AST_ROOT->semantic(ast::makeBaseEnvTable());
  }

  printf("\n\n~~~~~~ DEBUGGING AST ENDED ~~~~~~\n\n");
}

void debug() {
  debug(TIGER_DEFAULT_INPUT);
}
}

int CODES[] = {
    NO_ERROR, // test1.tig
    NO_ERROR,
    NO_ERROR,
    NO_ERROR,
};

static int __TIGER_UNIT_TEST = describe("ast tree", [] {
//  return ast::debug("../mocks/test4.tig");

  it("should parse ast correct", [] {
    for (auto i: TIGS) {
      assert(ast::parse(i.c_str())==0);
    }
  });

  it("should semantic check correct", [] {
      auto base_env = ast::makeBaseEnvTable();
      auto index = 0;
      auto length = sizeof(CODES) / sizeof(int);

      for (auto i: TIGS) {
        ast::parse(i.c_str());

        printf("parsing %s\n", i.c_str());
        if (ast::AST_ROOT) {
          auto result = ast::AST_ROOT->semantic(base_env);
          assert(result->code == CODES[index]);
        }

        index++;

        if (index >= length) break;
      }
  });

});
