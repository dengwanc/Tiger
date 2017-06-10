#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include "../src/utils/index.h"
#include "../src/lexical/error.h"
#include "../src/lexical/tokens.h"

using namespace std;

const char* toknames[] = {
        "ID", "STRING", "INT", "COMMA", "COLON", "SEMICOLON", "LPAREN",
        "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS",
        "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT", "GE",
        "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE", "FOR",
        "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
        "VAR", "TYPE"
};
const char* tokname(int tok) {return tok < 257 || tok > 299 ? "BAD_TOKEN" : toknames[tok-257];}


YYSTYPE yylval; /* the yylval ... record current token some info */

int yylex(void); /* prototype for the lexing function */

static int __TIGER_UNIT_TEST = describe("lexical", [] {

    it("should get token correct", [] {
        int tok;
        EM_reset(String("../mocks/merge.tig"));
        for(;;) {
            tok=yylex();
            if (tok==0) break;

            if(strcmp("BAD_TOKEN", tokname(tok)) == 0) assert("Invalid Token");

            // here is unit-test i'm lazy --
            if(strcmp("WHILE", tokname(tok)) != 0) assert("Program failed");

            switch (tok) {
                case ID: case STRING:
                    printf("%10s %4d %s\n",tokname(tok),EM_tokPos,yylval.sval);
                    break;
                case INT:
                    printf("%10s %4d %d\n",tokname(tok),EM_tokPos,yylval.ival);
                    break;
                default:
                    printf("%10s %4d\n",tokname(tok),EM_tokPos);
            }
        }
    });

});
