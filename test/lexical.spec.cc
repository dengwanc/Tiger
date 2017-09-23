#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/ast/y.tab.hh"
#include "global.h"

/**
 * lex native interface
 * https://www.ibm.com/support/knowledgecenter/en/SSLTBW_1.13.0/com.ibm.zos.r13.bpxa600/rtine.htm
 */
extern int yylex();
extern FILE *yyin;

static const char *tokens[] = {
    "ID", "STRING", "INT", "ASSIGN", "OR",
    "AND", "GE", "GT", "LE", "LT", "NEQ",
    "EQ", "MINUS", "PLUS", "DIVIDE", "TIMES",
    "ELSE", "OF", "DO", "UMINUS", "COMMA",
    "COLON", "SEMICOLON", "LPAREN", "RPAREN",
    "LBRACK", "RBRACK", "LBRACE", "RBRACE",
    "DOT", "ARRAY", "IF", "THEN", "WHILE",
    "FOR", "TO", "LET", "IN", "END", "BREAK",
    "NIL", "FUNCTION", "VAR", "TYPE", "REAL"
};

static const char *getName(int tok) {
  return tok < 258 || tok > 302 ? "BAD_TOKEN" : tokens[tok - 258];
}

void debug(const char *path) {
  int token;
  path = path ? path : TIGER_DEFAULT_INPUT;

  lexical::reset(path);

  while ((token = yylex(), token)) {
    /*cout << token << endl;*/

    if (streq("BAD_TOKEN", getName(token))) {
      /* do something with BDA_TOKEN */
    }

    switch (token) {
    case ID:
    case STRING:printf("%10s %s\n", getName(token), yylval.sval);
      break;
    case INT:printf("%10s %d\n", getName(token), yylval.ival);
      break;
    case REAL:printf("%10s %f\n", getName(token), yylval.dval);
      break;
    default:printf("%10s \n", getName(token));
    }
  }

  fclose(yyin);
}

static int __TIGER_UNIT_TEST = describe("lexical", [] {
  it("should parse token correct", [] {
    for (auto i: TIGS) {
      lexical::parse(i.c_str());
      assert(!hasErrors());
    }

    // debug(nullptr); /* if want observe */
  });

});
