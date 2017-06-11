#include <cstdio>
#include <cassert>
#include <iostream>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/lexical/tokens.h"
#include "global.h"

using namespace std;

/**
 * lex native interface
 * https://www.ibm.com/support/knowledgecenter/en/SSLTBW_1.13.0/com.ibm.zos.r13.bpxa600/rtine.htm
 */
extern int yylex();
extern FILE* yyin;

static const char* tokens[] = {
        "ID", "STRING", "INT", "COMMA", "COLON", "SEMICOLON", "LPAREN",
        "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS",
        "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT", "GE",
        "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE", "FOR",
        "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
        "VAR", "TYPE", "REAL"
};

static const char* getName(int tok)
{
    return tok < 257 || tok > 300 ? "BAD_TOKEN" : tokens[tok-257];
}

void debug(const char* path)
{
    int token;
    path = path ? path : "../mocks/editable.tig";

    resetLex(path);

    while ((token = yylex(), token)) {

        if(streq("BAD_TOKEN", getName(token))) {
            /* do something with BDA_TOKEN */
        }

        switch (token) {
            case ID: case STRING:
                printf("%10s %s\n", getName(token), yylval.sval);
                break;
            case INT:
                printf("%10s %d\n", getName(token), yylval.ival);
                break;
            case REAL:
                printf("%10s %f\n", getName(token), yylval.dval);
                break;
            default:
                printf("%10s \n", getName(token));
        }
    }

    fclose(yyin);
}

static int __TIGER_UNIT_TEST = describe("lexical", [] {
    it("should parse token correct", [] {
        for (auto i: TIGS) {
            parse(i.c_str());



            assert(!hasErrors());
        }

         // debug(nullptr); /* if want observe */
    });

});
