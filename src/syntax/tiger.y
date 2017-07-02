%{
	#include "../utils/symbol.h"
	#include "index.h"

	using namespace syntax;

	extern int yylex();
%}



%union {
	int ival;
	double dval;
	char* sval;
	struct S_symbol_* sym;
	struct ValfieldList* valfields;
	struct ExprList* exprs;
	struct Expr* expr;
	struct Lvalue* lv;
	struct Funcdec* funcdec;
	struct TypefieldList* typefields;
	struct Declare* declare;
	struct DeclareList* declares;
	struct Type* type;
}

%token <sval> ID STRING
%token <ival> INT
%token <dval> REAL

%token
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
  LBRACE RBRACE DOT PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN ARRAY WHILE DO LET OF
  BREAK NIL FUNCTION VAR TYPE QUESTION

%type <expr> expr program record
%type <lv> lvalue
%type <declares> declares
%type <sym> id
%type <valfields> valfields
%type <exprs> exprlist arguments
%type <funcdec> funcdec
%type <typefields> typefields
%type <declare> declare typedec vardec
%type <type> type

%right ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left COLON
%right QUESTION
%nonassoc IFTHEN
%nonassoc NEGTIVE


%start program

%%

program: expr {ABSYN_ROOT = $1;}

declares:
	  declare declares {$$ = DeclareList4($1, $2);}
	| declare          {$$ = DeclareList4($1, nullptr);}

declare:
	  typedec {$$ = $1;}
	| vardec  {$$ = $1;}
	| funcdec {$$ = Declare4($1);}

typedec: TYPE id ASSIGN type {$$ = Declare4($2, $4);}

type:
	  id                       {$$ = Type4($1);}
	| ARRAY OF id              {$$ = ArrayType($3);}
	| LBRACE typefields RBRACE {$$ = Type4($2);}

typefields:
	  id COLON id COMMA typefields {$$ = TypefieldList4($1, $3, $5);}
	| id COLON id                  {$$ = TypefieldList4($1, $3, nullptr);}

vardec:
	  VAR id ASSIGN expr {$$ = Declare4($2, nullptr, $4);}
	| VAR id COLON id    {$$ = Declare4($2, $4, nullptr);}

funcdec:
	  FUNCTION id ASSIGN expr                   {$$ = Funcdec4($2, nullptr, nullptr, $4);}
	| FUNCTION id LPAREN typefields RPAREN expr {$$ = Funcdec4($2, $4, nullptr, $6);}

lvalue:
	  id                       {$$ = Lvalue4($1);}
	| lvalue DOT id             {$$ = Lvalue4($1, $3);}
	| lvalue LBRACK expr RBRACK {$$ = Lvalue4($1, $3);}

arguments:
	  expr COMMA arguments {$$ = ExprList4($1, $3);}
	| expr                 {$$ = ExprList4($1, nullptr);}
	|                      {$$ = nullptr;}

expr:
      lvalue                              {$$ = Expr4($1);}
	| INT                                 {$$ = Expr4($1);}
	| REAL                                {$$ = Expr4($1);}
	| STRING                              {$$ = Expr4($1);}
	| MINUS expr %prec NEGTIVE            {$$ = MinusExpr($2);}
	| id LPAREN arguments RPAREN          {$$ = Expr4($1, $3);}
	| expr PLUS expr                      {$$ = Expr4(Plus, $1, $3);}
	| expr MINUS expr                     {$$ = Expr4(Minus, $1, $3);}
	| expr TIMES expr                     {$$ = Expr4(Times, $1, $3);}
	| expr DIVIDE expr                    {$$ = Expr4(Divide, $1, $3);}
	| expr EQ expr                        {$$ = Expr4(Eq, $1, $3);}
	| expr NEQ expr                       {$$ = Expr4(Neq, $1, $3);}
	| expr GT expr                        {$$ = Expr4(Gt, $1, $3);}
	| expr LT expr                        {$$ = Expr4(Lt, $1, $3);}
	| expr GE expr                        {$$ = Expr4(Ge, $1, $3);}
	| expr LE expr                        {$$ = Expr4(Le, $1, $3);}
	| expr AND expr                       {$$ = AndExpr($1, $3);}
	| expr OR expr						  {$$ = OrExpr($1, $3);}
	| record                              {$$ = $1;}
	| LBRACK expr RBRACK id               {$$ = Expr4($4, $2);}
	| lvalue ASSIGN expr                  {$$ = Expr4($1, $3);}
	| expr QUESTION expr COLON expr       {$$ = Expr4($1, $3, $5);}
	| expr QUESTION expr %prec IFTHEN     {$$ = Expr4($1, $3, nullptr);}
	| WHILE expr LBRACE expr RBRACE       {$$ = Expr4($2, $4);}
	| BREAK                               {$$ = BreakExpr();}
	| LET declares LBRACE exprlist RBRACE {$$ = Expr4($2, $4);}
	| LPAREN expr RPAREN                  {$$ = $2;}
	| NIL                                 {$$ = NilExpr();}

exprlist:
	  expr SEMICOLON exprlist {$$ = ExprList4($1, $3);}
	| expr                    {$$ = ExprList4($1, nullptr);}

record: LBRACE valfields RBRACE id {$$ = Expr4($4, $2);}

valfields:
	  id COLON expr COMMA valfields {$$ = ValfieldList4($1, $3, $5);}
	| id COLON expr                 {$$ = ValfieldList4($1, $3, nullptr);}

id: ID {$$ = Symbol4($1);}

%%
