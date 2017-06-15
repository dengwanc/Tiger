#include "../lexical/index.h"
#include "../utils/index.h"

extern char* yytext;

static bool anyerrors = false;

void refresh()
{
  anyerrors = false;
}

bool hasErrors()
{
    return anyerrors;
}

void reportError(const char *message)
{
    anyerrors = true;

    const char* filename = getFilename();
    int linecount = getLine();
    int token_pos = getOffset();

    if (!empty(filename)) fprintf(stderr, "IN FILE %s ", filename);
    if (linecount - 1) {
        fprintf(stderr, " LINE %d.%d: \n", linecount, token_pos);
        fprintf(stderr, "%s\n", yytext);
    }

    fprintf(stderr, "%s", message);
    fprintf(stderr, "\n");
}

void reportBadToken()
{
    reportError("ILLEGAL TOKEN");
}

void reportBadSyntax()
{

}
