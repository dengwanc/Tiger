#include <assert.h>
#include <stdio.h>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/ast/index.h"
#include "global.h"

extern int yyparse(void);

namespace ast {
    int parse(const char *fname) {
        lexical::reset(fname);
        if (yyparse() == 0) {
            return 0;
        } else {
            return 1;
        }
    }

    void debug(const char* file) {
        printf("\n~~~~~~ DEBUGGING AST STARTED ~~~~~~\n\n");
        assert(ast::parse(file) == 0);
        if (ast::AST_ROOT == nullptr) {
            printf("⚠️  %s has no content", TIGER_DEFAULT_INPUT);
        } else {
            ast::AST_ROOT->print();
        }

        printf("\n\n~~~~~~ DEBUGGING AST ENDED ~~~~~~\n\n");
    }

    void debug()
    {
        debug(TIGER_DEFAULT_INPUT);
    }
}

static int __TIGER_UNIT_TEST = describe("ast tree", [] {

    it("should parse ast correct", [] {
        for (auto i: TIGS) {
            if (i == "../mocks/merge.tig") continue;
            if (i == "../mocks/queens.tig") continue;
            assert(ast::parse(i.c_str()) == 0);
        }

        // ast::debug(); /* if want observe */
    });
});
