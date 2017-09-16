#include <string.h>
#include <stdio.h>
#include <iostream>
#include "index.h"
#include "../utils/index.h"
#include "../error/index.h"
#include "../ast/y.tab.hh"

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

int lexical::getLine()
{
    return linecount;
}

int lexical::getOffset()
{
    return token_pos;
}


void lexical::adjust()
{
    token_pos += yyleng;
}

void lexical::newline(void)
{
    linecount++;
    token_pos = 0;
}


void lexical::inc()
{
    comment_nest++;
}

void lexical::dec()
{
    comment_nest--;
}

bool lexical::hasComment()
{
    return comment_nest != 0;
}


void lexical::initString() {
    remain = MAX_LENGTH - 1;
    strptr = str;
}

void lexical::appendChar(int ch)
{
    if (!remain) throw COMPILER_STRING_OVER;

    *strptr++ = (char)ch;
    remain--;
}

void lexical::appendStr(char *s)
{
    int t = strlen(s);

    if (remain < t) throw COMPILER_STRING_OVER;

    do { *strptr++ = *s++; } while(*s);
    remain -= t;
}

void lexical::endString()
{
    if (!remain) throw COMPILER_STRING_OVER;

    *strptr++ = '\0';
}

void lexical::recordString()
{
    yylval.sval = String(str);
}



void lexical::recordId()
{
    yylval.sval = String(yytext);
}

void lexical::recordInt()
{
    yylval.ival = atoi(yytext);
}

void lexical::recordReal()
{
    yylval.dval = atof(yytext);
}


static const char* filename = "";

const char* lexical::getFilename()
{
    return filename;
}

void lexical::reset(const char *path)
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

void lexical::parse(const char* path/*, std::function<void()> reject*/)
{
    int token;

    reset(path);

    while ((token = yylex(), token)) {
        if (token < 257 || token > 300) {
            /* reject(); */
        }
    }

    fclose(yyin);
}
