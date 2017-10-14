#include <stdio.h>
#include "global.h"
#include "../src/ast/index.h"
#include "../src/semantic/errcode.h"

static void debug(const char *file) {
  verboseSemanticError();
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

static void debug() {
  debug(TIGER_DEFAULT_INPUT);
}

static int ERR_CODES[] = {
    NO_ERROR, // test1.tig
    NO_ERROR,
    NO_ERROR,
    NO_ERROR,
    NO_ERROR,
    NO_ERROR, // test6.tig
    NO_ERROR,
    NO_ERROR,
    IF_EXPR_ERROR1,
    NO_ERROR,
    NO_ERROR, // test11.tig
    NO_ERROR,
    OPERATOR_DIFF_TYPE,
    OPERATOR_DIFF_TYPE,
    NO_ERROR,
    NO_ERROR, // test16.tig
    NO_ERROR,
    NO_ERROR,
    VAR_NOT_DEFINED,
    VAR_NOT_DEFINED,
    NO_ERROR, // test21.tig
    FILED_ERR1,
    NOT_ASSIGNABLE,
    NOT_ARRAY,
    FILED_ERR2,
    NO_ERROR, // test26.tig
    OPERATOR_DIFF_TYPE,
    NOT_ASSIGNABLE,
    NOT_ASSIGNABLE,
    NO_ERROR,
    NO_ERROR, // test31.tig
    NOT_ASSIGNABLE,
    RECORD_EXPR_ERR1,
    ARG_NOT_MATCH,
    SHORT_ARG,
    NO_ERROR, // test36.tig
    SURPLUS_ARG,
    DUPLICATE_TYPE,
    DUPLICATE_FUNC,
    DUPLICATE_TYPE,
    NO_ERROR, // test41.tig
    NO_ERROR,
    OPERATOR_DIFF_TYPE,
    VAR_DECLARE_ERR1,
    VAR_DECLARE_ERR1,
    NO_ERROR, // test46.tig
    DUPLICATE_TYPE,
    DUPLICATE_FUNC,
    VAR_DECLARE_ERR1,
};

static int __TIGER_UNIT_TEST = describe("semantic check", [] {
  /** good way to debug, without effect === */
  // return debug("../mocks/test49.tig");
  /** debug part ========================== */

  slientSemanticError();

  it("should semantic check correct", [] {
    auto base_env = ast::makeBaseEnvTable();
    auto index = 0;
    auto length = sizeof(ERR_CODES) / sizeof(int);

    for (auto i: TIGS) {
      ast::parse(i.c_str());

      if (ast::AST_ROOT) {
        ast::AST_ROOT->semantic(base_env);
        assert(ast::getSemanticErrorCode() == ERR_CODES[index]);
      }

      index++;

      if (index >= length) break;
    }
  });
});
