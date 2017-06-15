#include <string.h>
#include <stdio.h>
#include <iostream>
#include "index.h"
#include "../utils/index.h"
#include "../error/index.h"

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
static int linecount = 1;
static char* strptr; /* pointer to behind char array `str` */
static char str[MAX_LENGTH]; /* string token cache */
static int remain;
static int comment_nest = 0;

int getLine()
{
    return linecount;
}

int getOffset()
{
    return token_pos;
}
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


void adjust()
{
    token_pos += yyleng;
}

void newline(void)
{
    linecount++;
    token_pos = 0;
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


static const char* filename = "";

const char* getFilename()
{
    return filename;
}

void resetLex(const char* path)
{
    refresh();

    filename = path;
    linecount = 1;

    yyin = fopen(path, "r");

    if (!yyin) {
        using namespace std;
        std::cout << (string("CAN NOT OPEN FILE ") + string(path)) << std::endl;
        std::exit(1);
    }
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
