%{

/**
 * The Meanings of
 * lex macro `BEGIN` and `%x`, `%s`
 * reference here:
 * ftp://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html#SEC11
 */

#include "index.h"
#include "../ast/y.tab.hh"
#include "../error/index.h"

using namespace lexical;

/**
 * End File Input
 * http://www.cnblogs.com/xzpp/p/4638071.html
 */
extern "C" int yywrap() { return 1; }

%}

id [_$A-Za-z][_?A-Za-z0-9]*
int [0-9]+
real [0-9]+\.[0-9]+
ws [ \t]+

%x comment
%x string

%%

","      {adjust(); return COMMA;}
"="      {adjust(); return ASSIGN;}
":"      {adjust(); return COLON;}
";"      {adjust(); return SEMICOLON;}
"("      {adjust(); return LPAREN;}
")"      {adjust(); return RPAREN;}
"{"      {adjust(); return LBRACE;}
"}"      {adjust(); return RBRACE;}
"["      {adjust(); return LBRACK;}
"]"      {adjust(); return RBRACK;}
"."      {adjust(); return DOT;}
"+"      {adjust(); return PLUS;}
"-"      {adjust(); return MINUS;}
"*"      {adjust(); return TIMES;}
"/"      {adjust(); return DIVIDE;}
"=="     {adjust(); return EQ;}
"<>"     {adjust(); return NEQ;}
"<="     {adjust(); return LE;}
"<"      {adjust(); return LT;}
">="     {adjust(); return GE;}
">"      {adjust(); return GT;}
"&"      {adjust(); return AND;}
"|"      {adjust(); return OR;}
array    {adjust(); return ARRAY;}
break    {adjust(); return BREAK;}
function {adjust(); return FUNCTION;}
let      {adjust(); return LET;}
in       {adjust(); return IN;}
end      {adjust(); return END;}
of       {adjust(); return OF;}
nil      {adjust(); return NIL;}
type     {adjust(); return TYPE;}
var      {adjust(); return VAR;}
while    {adjust(); return WHILE;}
if       {adjust(); return IF;}
then     {adjust(); return THEN;}
else     {adjust(); return ELSE;}

{id}     {adjust();  recordId(); return ID;}
{int}    {adjust();  recordInt(); return INT;}
{real}   {adjust(); recordReal(); return REAL;}
{ws}     {adjust(); continue;}
\n       {adjust(); newline(); continue;}

"/*"                    {adjust(); inc(); BEGIN comment;}
<comment>"/*"           {adjust(); inc(); BEGIN comment;}
<comment>"*/"           {adjust(); dec(); if(!hasComment()) {BEGIN 0;}}
<comment>(.)            {adjust(); continue;}
<comment>\n             {adjust(); newline(); continue;}

\"                      {adjust(); initString(); BEGIN string;}
<string>\\              {adjust(); appendChar(0x5c);}
<string>\\\"            {adjust(); appendChar(0x22);}
<string>\\n             {adjust(); appendChar(0x0A);}
<string>\\t             {adjust(); appendChar(0x09);}
<string>\\[0-9]{3}      {adjust(); appendChar(atoi(yytext));}
<string>\"              {adjust(); endString(); recordString(); BEGIN 0; return STRING;}
<string>\n              {adjust(); newline(); appendChar(0x0A);}
<string>{ws}            {adjust(); appendStr(yytext);}
<string>[^\\" \t\n]+    {adjust(); appendStr(yytext);}

.                       {adjust(); reportBadToken();}

%%
