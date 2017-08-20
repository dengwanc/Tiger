#ifndef TIGER_ERROR
#define TIGER_ERROR

struct Location {
    int line;
    int offset;
};

bool hasErrors();
void reportError(const char* message);
void reportBadToken();
void refresh();
void yyerror(const char*);
void reportSemanticError(char* msg, struct Location &lo);

#endif

