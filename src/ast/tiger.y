%{
	extern int yylex();
%}

%code requires {
	#include "../utils/symbol.h"
	#include "index.h"
	using namespace ast;
}

%error-verbose

%union {
	int ival;
	double dval;
	char *sval;
	Symbol sym;

	Expr *expr;
    Lvalue *lv;
    Type *type;
    Declare *declare;

	struct ValfieldList* valfields;
	struct ExprList* exprs;
	struct TypefieldList* typefields;
	struct DeclareList* declares;

}

%token <sval> ID STRING
%token <ival> INT
%token <dval> REAL

%token
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
  LBRACE RBRACE DOT PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN ARRAY WHILE LET IN END OF
  BREAK NIL FUNCTION VAR TYPE IF THEN ELSE

%type <expr> expr program record
%type <lv> lvalue
%type <declares> declares
%type <sym> id
%type <valfields> valfields
%type <exprs> exprlist arguments
%type <typefields> typefields
%type <declare> declare typedec vardec funcdec
%type <type> type

%right ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc NEGTIVE

%start program

%%

program:
	expr               {AST_ROOT = $1;}
	|                  {AST_ROOT = nullptr;}

declares:
	  declare declares {$$ = DeclareList4($1, $2);}
	| declare          {$$ = DeclareList4($1, nullptr);}

declare:
	  typedec {$$ = $1;}
	| vardec  {$$ = $1;}
	| funcdec {$$ = $1;}

typedec: TYPE id ASSIGN type {$$ = new TypeDeclare($2, $4);}

type:
	  id                       {$$ = new NameType($1);}
	| ARRAY OF id              {$$ = new ArrayType($3);}
	| LBRACE typefields RBRACE {$$ = new RecordType($2);}

typefields:
	  id COLON id COMMA typefields {$$ = TypefieldList4($1, $3, $5);}
	| id COLON id                  {$$ = TypefieldList4($1, $3, nullptr);}

vardec:
	  VAR id ASSIGN expr          {$$ = new VarDeclare($2, nullptr, $4);}
	| VAR id COLON id             {$$ = new VarDeclare($2, $4, nullptr);}
	| VAR id COLON id ASSIGN expr {$$ = new VarDeclare($2, $4, $6);}

funcdec:
	  FUNCTION id ASSIGN expr                   {$$ = new FunctionDeclare($2, nullptr, nullptr, $4);}
	| FUNCTION id LPAREN typefields RPAREN expr {$$ = new FunctionDeclare($2, $4, nullptr, $6);}

lvalue:
	  id                        {$$ = new SimpleLvalue($1);}
	| lvalue DOT id             {$$ = new FieldLvalue($1, $3);}
	| lvalue LBRACK expr RBRACK {$$ = new SubscriptLvalue($1, $3);}

arguments:
	  expr COMMA arguments {$$ = ExprList4($1, $3);}
	| expr                 {$$ = ExprList4($1, nullptr);}
	|                      {$$ = nullptr;}

expr:
      lvalue                                  {$$ = new LvalueExpr($1);}
    | LBRACE exprlist RBRACE                  {$$ = new SeqExpr($2);}
	| INT                                     {$$ = new IntExpr($1);}
	| REAL                                    {$$ = new RealExpr($1);}
	| STRING                                  {$$ = new StringExpr($1);}
	| MINUS expr %prec NEGTIVE                {$$ = MinusExpr($2);}
	| id LPAREN arguments RPAREN              {$$ = new CallExpr($1, $3);}
	| expr PLUS expr                          {$$ = new OpExpr(Plus, $1, $3);}
	| expr MINUS expr                         {$$ = new OpExpr(Minus, $1, $3);}
	| expr TIMES expr                         {$$ = new OpExpr(Times, $1, $3);}
	| expr DIVIDE expr                        {$$ = new OpExpr(Divide, $1, $3);}
	| expr EQ expr                            {$$ = new OpExpr(Eq, $1, $3);}
	| expr NEQ expr                           {$$ = new OpExpr(Neq, $1, $3);}
	| expr GT expr                            {$$ = new OpExpr(Gt, $1, $3);}
	| expr LT expr                            {$$ = new OpExpr(Lt, $1, $3);}
	| expr GE expr                            {$$ = new OpExpr(Ge, $1, $3);}
	| expr LE expr                            {$$ = new OpExpr(Le, $1, $3);}
	| expr AND expr                           {$$ = AndExpr($1, $3);}
	| expr OR expr						      {$$ = OrExpr($1, $3);}
	| record                                  {$$ = $1;}
	| LBRACK expr RBRACK id                   {$$ = new ArrayExpr($4, $2);}
	| lvalue ASSIGN expr                      {$$ = new AssignExpr($1, $3);}
	| IF expr THEN exprlist ELSE exprlist END {$$ = new IfExpr($2, $4, $6);}
	| IF expr THEN exprlist END               {$$ = new IfExpr($2, $4, nullptr);}
	| WHILE expr LBRACE exprlist RBRACE       {$$ = new WhileExpr($2, $4);}
	| BREAK                                   {$$ = new BreakExpr();}
	| LET declares IN exprlist END            {$$ = new LetExpr($2, $4);}
	| LPAREN expr RPAREN                      {$$ = $2;}
	| NIL                                     {$$ = new NilExpr();}

exprlist:
	  expr SEMICOLON exprlist {$$ = ExprList4($1, $3);}
	| expr                    {$$ = ExprList4($1, nullptr);}

record: LBRACE valfields RBRACE id {$$ = new RecordExpr($4, $2);}

valfields:
	  id COLON expr COMMA valfields {$$ = ValfieldList4($1, $3, $5);}
	| id COLON expr                 {$$ = ValfieldList4($1, $3, nullptr);}

id: ID {$$ = Symbol4($1);}

%%
