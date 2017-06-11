#ifndef TIGER_LEXICAL
#define TIGER_LEXICAL

#include<iostream>

void adjust();
void initString();
void appendChar(int ch);
void appendStr(char *s);
void endString();
void inc();
void dec();
bool hasComment();
void recordString();
void recordId();
void recordInt();
void recordReal();
void newline();
bool hasErrors();
void reportError(int, const char *);
void resetLex(const char *path);
void reportBadToken();
void parse(const char* path/*, std::function<void()> reject*/);

typedef union  {
    int ival; 	 // record int token value
    char* sval;  // record string token value
    double dval; // record real token value
} YYSTYPE;

extern YYSTYPE yylval;

#endif