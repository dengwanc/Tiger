#include "index.h"
#include "../utils/index.h"

using namespace lexical;

extern char *yytext;

static bool anyerrors = false;
static bool slient = false;

void yyerror(const char *msg) {
  char last[512];
  auto line = lexical::getLine();
  auto file = lexical::getFilename();
  sprintf(last, "`%s` %s (%s:%d)", yytext, msg, file, line);
  crash(last);
}

void clearErrors() {
  anyerrors = false;
}

bool hasErrors() {
  return anyerrors;
}

void reportError(const char *message) {
  anyerrors = true;

  const char *filename = getFilename();
  int linecount = getLine();
  int token_pos = getOffset();

  if (!empty(filename))
    fprintf(stderr, "IN FILE %s ", filename);
  if (linecount) {
    fprintf(stderr, " LINE %d.%d: \n", linecount, token_pos);
    fprintf(stderr, "%s\n", yytext);
  }

  fprintf(stderr, "%s", message);
  fprintf(stderr, "\n");
}

void reportBadToken() {
  reportError("ILLEGAL TOKEN");
}

void reportSemanticError(char *msg, struct Location &lo) {
  if (slient) return;
  anyerrors = true;
  fprintf(stderr, "%s LINE %d.%d: \n", getFilename(), lo.line, lo.offset);
  fprintf(stderr, "%s\n", msg);
}

void slientSemanticError() {
  slient = true;
}

void verboseSemanticError() {
  slient = false;
}
