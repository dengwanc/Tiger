#ifndef TIGER_ERROR
#define TIGER_ERROR

bool hasErrors();
void reportError(const char* message);
void reportBadToken();
void refresh();
void yyerror(const char*);

#endif

