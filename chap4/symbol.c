#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "symbol.h"
#include "table.h"

struct S_symbol_ {string name; S_symbol next;};

static S_symbol mksymbol(string name, S_symbol next)
{S_symbol s=checked_malloc(sizeof(*s));
 //s->name=name; //This fuck author err exp; (fuck the author)
 s->name=checked_malloc(sizeof(strlen(name)) + 1);
 strcpy(s->name, name);
 //s->next=next;
 return s; //
}

#define SIZE 1000  /* should be prime */

static S_symbol hashtable[SIZE];

static unsigned int hash(char *s0)
{
	unsigned int h=0; char *s;
    for(s=s0; *s; s++){
    	if (isupper(*s) | *s == '_') h -= 'A'; else h -= 'a';
        h = h + *s;
    }
    return h;
}
 
static int streq(string a, string b)
{
 return !strcmp(a,b);
}
/*
 * hashtable is a global-var save array of S_symbol
 * 
 */
S_symbol S_Symbol(string name)
{
	//printf("fuck %d fuck\n", hashtable[7]);
	int index= hash(name) % SIZE;
	//printf("%d\n",index);
	S_symbol syms = hashtable[index], sym;
	/*
	for(sym=syms; sym; sym=sym->next)
		if (streq(sym->name,name)) return sym;
	sym = mksymbol(name,syms);
	hashtable[index]=sym;
	
	*/
	//printf("%d\n",index);
	if (syms) return syms;
	sym = mksymbol(name, syms);
	hashtable[index] = sym;
	//printf("this is is S_Symbol %s \n",hashtable[0]->name);
	//S_distable();
	return sym;
}

void S_distable()
{
	int i;
	S_symbol t;
	for (i = 0; i < SIZE; i++){
		//t = hashtable[7];
		//t = checked_malloc(sizeof(*t));
		//printf("%d\nssss", t);
		if (hashtable[i]) printf("%s\n", hashtable[i]->name);
		//for ( ; t; t=t->next) printf("%s->", t->name);
		//printf("0\n");
	}
	//printf("fuck %d fuck", hashtable[7]);
}
 
string S_name(S_symbol sym)
{
 //printf("this is in S_name %s \n", sym->name);
 return sym->name;
}

S_table S_empty(void) 
{ 
 return TAB_empty();
}

void S_enter(S_table t, S_symbol sym, void *value) {
  TAB_enter(t,sym,value);
}

void *S_look(S_table t, S_symbol sym) {
  return TAB_look(t,sym);
}

static struct S_symbol_ marksym = {"<mark>",0};

void S_beginScope(S_table t)
{ 
	S_enter(t,&marksym,NULL);
}

void S_endScope(S_table t)
{
  S_symbol s;
  do s=TAB_pop(t);
  while (s != &marksym);
}

void S_dump(S_table t, void (*show)(S_symbol sym, void *binding)) {
  TAB_dump(t, (void (*)(void *, void *)) show);
}

