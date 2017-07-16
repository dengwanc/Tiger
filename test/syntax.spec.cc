#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/syntax/index.h"
#include "../src/syntax/dump.h"
#include "global.h"

extern int yyparse(void);

namespace bug949 {
    int parse(const char *fname) {
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
        assert(bug949::parse(TIGER_DEFAULT_INPUT) == 0);
        using namespace syntax;
        printf("\n~~~~~~ DEBUGGING AST STARTED ~~~~~~\n\n");
        printExpr(ABSYN_ROOT);
        printf("\n\n~~~~~~ DEBUGGING AST ENDED ~~~~~~\n");
    });
});
