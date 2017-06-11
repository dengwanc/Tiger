#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include "index.h"
#include "../utils/index.h"

/**
 * the max string size is 4Kb
 * define by random
 */
#define MAX_LENGTH (1024*4)

extern int yyleng; // simple equals `strlen(yytext)`
extern char* yytext;
extern FILE* yyin;
extern int yylex();

/**
 * the value of yylex so named `yylval`
 * for record current token info in a `yylex()` circle
 */
YYSTYPE yylval;

/**
 * the index of token in the line
 * ex: `3 > 4@`
 * the error msg will be: "error in line 1:6"
 */
static int token_pos = 0;
static char* strptr; /* pointer to behind char array `str` */
static char str[MAX_LENGTH]; /* string token cache */
static int remain;
static int comment_nest = 0;

/**
 * flex native interface
 * if return 1
 * lex stop parse
 */
extern "C" int yywrap()
{
    token_pos = 0;
    return 1;
}

/**
 * record line position
 */
void adjust()
{
    token_pos += yyleng;
}



void inc()
{
    comment_nest++;
}

void dec()
{
    comment_nest--;
}

bool hasComment()
{
    return comment_nest != 0;
}



void initString() {
    remain = MAX_LENGTH - 1;
    strptr = str;
}

void appendChar(int ch)
{
    if (!remain) throw COMPILER_STRING_OVER;

    *strptr++ = (char)ch;
    remain--;
}

void appendStr(char *s)
{
    int t = strlen(s);

    if (remain < t) throw COMPILER_STRING_OVER;

    do { *strptr++ = *s++; } while(*s);
    remain -= t;
}

void endString()
{
    if (!remain) throw COMPILER_STRING_OVER;
    
    *strptr++ = '\0';
}

void recordString()
{
    yylval.sval = String(str);
}



void recordId()
{
    yylval.sval = yytext;
}

void recordInt()
{
    yylval.ival = atoi(yytext);
}

void recordReal()
{
    yylval.dval = atof(yytext);
}


static bool anyerrors = false;
static std::string filename = "";
static int linecount = 1;

void newline(void)
{
    linecount++;
    token_pos = 0;
}

bool hasErrors()
{
    return anyerrors;
}

void reportError(int pos, const char *message)
{
    int num = linecount;

    anyerrors = true;

    if (!filename.empty()) fprintf(stderr, "IN FILE %s ", filename.c_str());
    if (linecount - 1) {
        fprintf(stderr, " LINE %d.%d: \n", linecount, token_pos);
        fprintf(stderr, "%s\n", yytext);
    }

    fprintf(stderr, "%s", message);
    fprintf(stderr, "\n");
}

void reportError(std::string msg)
{
    std::cout << msg << std::endl;
}

void resetLex(const char* path)
{

    anyerrors = false;
    filename = path;
    linecount = 1;

    yyin = fopen(path, "r");

    if (!yyin) {
        using namespace std;
        reportError(string("CAN NOT OPEN FILE ") + string(path));
        std::exit(1);
    }
}

void reportBadToken()
{
    reportError(token_pos, "ILLEGAL TOKEN");
}

void parse(const char* path/*, std::function<void()> reject*/)
{
    int token;

    resetLex(path);

    while ((token = yylex(), token)) {
        if (token < 257 || token > 300) {
            /* reject(); */
        }
    }

    fclose(yyin);
}
