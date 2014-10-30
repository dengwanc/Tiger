#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "types.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"

struct Tr_level_ {
	Tr_level parent;
	Temp_label name;
	F_frame frame;
	Tr_accessList formals;
};

struct Tr_access_ {
	Tr_level level;
	F_access access;
};

static Tr_accessList makeFormalAccessList(Tr_level);
static Tr_access Tr_Access(Tr_level, F_access);
/**debug info**/
static void display_l(Tr_level);
static void display_ac(Tr_access);

/*******IR*******/
struct Cx {
	patchList trues;
	patchList falses;
	T_stm stm;
};

struct Tr_exp_ {
	enum {Tr_ex, Tr_nx, Tr_cx} kind;
	union {
		T_exp ex;    /*exp*/
		T_stm nx;    /*non-exp*/
		struct Cx cx;/*condition-stm*/ 
	} u;
};

struct patchList_ {
	Temp_label * head; /*???why save a point-to-Temp_label*/
	patchList tail;
};

struct Tr_expList_ {
	Tr_exp head;
	Tr_expList tail;
};

static Tr_exp Tr_Ex(T_exp);
static Tr_exp Tr_Nx(T_stm);
static Tr_exp Tr_Cx(patchList, patchList, T_stm);
static patchList PatchList(Temp_label *, patchList);
static T_exp unEx(Tr_exp);
static T_stm unNx(Tr_exp);
static struct Cx unCx(Tr_exp);
static void doPatch(patchList, Temp_label);
static patchList joinPatch(patchList, patchList);
static Tr_exp Tr_StaticLink(Tr_level, Tr_level);


Tr_expList Tr_ExpList(Tr_exp h, Tr_expList t) {
    Tr_expList l = checked_malloc(sizeof(*l));
	l->head = h;
	l->tail = t;
	return l;
}

void Tr_expList_prepend(Tr_exp h, Tr_expList * l) {
	/* add a newhead at a old expList, alter the point-content*/
	Tr_expList newhead = Tr_ExpList(h, NULL);
	newhead->tail = *l;
	*l = newhead;
}

static Tr_exp Tr_Ex(T_exp exp) {
	Tr_exp e = checked_malloc(sizeof(*e));
	e->kind = Tr_ex;
	e->u.ex = exp;
	return e;
}

static Tr_exp Tr_Nx(T_stm stm) {
	Tr_exp e = checked_malloc(sizeof(*e));
	e->kind = Tr_nx;
	e->u.nx = stm;
	return e;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm) {
	Tr_exp e = checked_malloc(sizeof(*e));
	e->kind = Tr_cx;
	e->u.cx.trues  = trues;
	e->u.cx.falses = falses;
	e->u.cx.stm    = stm;
	return e;
}

static T_exp unEx(Tr_exp e) {
	/*trans Tr_exp to T_exp*/
	switch (e->kind) {
	case Tr_ex:
		return e->u.ex;
	case Tr_nx:
		return T_Eseq(e->u.nx, T_Const(0));
	case Tr_cx:
		break;
		//TODO
	}
	assert(0 && "only 3 condition");
}

static T_stm unNx(Tr_exp e) {
	/*trans Tr_exp to T_stm*/
	switch (e->kind) {
    case Tr_ex:
		return T_Exp(e->u.ex);
	case Tr_nx:
		return e->u.nx;
	case Tr_cx:
		break;
		//TODO
	}
	assert(0);
}

static struct Cx unCx(Tr_exp e){
	switch (e->kind) {
	case Tr_cx:
		return e->u.cx;
	case Tr_ex:
		break;
		//TODO
	case Tr_nx:
		assert(0);
		/*this should not occur*/
	}
	assert(0);
}

Tr_exp Tr_simpleVar(Tr_access ac, Tr_level l) {
	/* */
	T_exp addr = T_Temp(F_FP());
	while (l && l != ac->level->parent) {
		F_access sl = F_formals(l->frame)->head;
		addr = F_Exp(sl, addr);
		l = l->parent;
	}
	return Tr_Ex(addr);
}

Tr_exp Tr_fieldVar(Tr_exp base, int offs) {
	/* return base + offs * WORD-SIZE */
	return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(base), T_Const(offs * F_WORD_SIZE))));
}

Tr_exp Tr_subscriptVar(Tr_exp base, Tr_exp index) {
	/* return base + index * WORD-SIZE */
	return Tr_Ex(T_Mem(T_Binop(T_plus,
					           unEx(base),
							   T_Binop(T_mul, unEx(index), T_Const(F_WORD_SIZE)))));
}

static F_fragList fragList       = NULL;

static F_fragList stringFragList = NULL;
Tr_exp Tr_stringExp(string s) {
	/*const-string is a label point a addr, like {str-num,"..."}*/
	Temp_label slabel = Temp_newlabel();
	F_frag fragment = F_StringFrag(slabel, s);
	stringFragList = F_FragList(fragment, stringFragList);
	return Tr_Ex(T_Name(slabel));
}

Tr_exp Tr_intExp(int i) {
	return Tr_Ex(T_Const(i));
}

Tr_exp Tr_doubleExp(float f) {
	return Tr_Ex(T_Double(f));
}

static Temp_temp nilTemp = NULL;
Tr_exp Tr_nilExp() {
	if (!nilTemp) {
		nilTemp = Temp_newtemp();
		//TODO record init
	}
	return Tr_Ex(T_Temp(nilTemp));
}

Tr_exp Tr_noExp() {
	/*const 0 stand non-val-exp*/
	return Tr_Ex(T_Const(0));
}

static Tr_exp Tr_StaticLink(Tr_level now, Tr_level def) {
	/*get call-function's static*/
	T_exp addr = T_Temp(F_FP());/*???*/
	while(now && (now != def->parent)) { /*???*/
		F_access sl = F_formals(now->frame)->head;
		addr = F_Exp(sl, addr);
		now = now->parent;
	}
	return Tr_Ex(addr);
}

static T_expList Tr_expList_convert(Tr_expList l) {
	/*trans Tr_expList to T_expList*/
	T_expList h = NULL, t = NULL;
	for (; l; l = l->tail) {
		T_exp tmp = unEx(l->head);
		if (h) {
			t = T_ExpList(tmp, NULL);
			t = t->tail;
		} else {
			h = T_ExpList(tmp, NULL);
			t = h->tail;
		}	
	}
	return h;
}

Tr_exp Tr_callExp(Temp_label label, Tr_level fun, Tr_level call, Tr_expList * l) {
	T_expList args = NULL;
	if (*l) {
		Tr_expList_prepend(Tr_StaticLink(call, fun), l);
		args = Tr_expList_convert(*l);
	} 
	return Tr_Ex(T_Call(T_Name(label), args));
}

static void doPatch(patchList t, Temp_label label) {
	/*for the list every item value the same label*/
	for (; t; t = t->tail) {
		*(t->head) = label;
	}
}

static patchList joinPatch(patchList fir, patchList scd) {
	/*add patch-List in the tail*/
	if (!fir) return scd;
	for (; fir->tail; fir = fir->tail);
	fir->tail = scd;
	return fir;
}

F_fragList Tr_getResult() {/*link stringFragList -> fragList */
	F_fragList cur = NULL, prev = NULL;
	for (cur = stringFragList; cur; cur = cur->tail)
		prev = cur;
	if (prev) prev->tail = fragList;
	return stringFragList ? stringFragList : fragList;
}
/*******IR*******/

Tr_level Tr_newLevel(Tr_level p, Temp_label n, U_boolList f) {
	Tr_level l = checked_malloc(sizeof(*l));
	l->parent = p;
	l->name = n;
	l->frame = F_newFrame(n, U_BoolList(TRUE, f));
	l->formals = makeFormalAccessList(l);
	//display_l(l);
	return l;
}

Tr_access Tr_allocLocal(Tr_level l, bool escape) {
	Tr_access a = checked_malloc(sizeof(*a));
	a->level = l;
	a->access = F_allocLocal(l->frame, escape);
	//display_ac(a);
	return a;
}

Tr_accessList Tr_AccessList(Tr_access h, Tr_accessList t) {
	Tr_accessList al = checked_malloc(sizeof(*al));
	al->head = h;
	al->tail = t;
	return al;
} 

Tr_accessList Tr_formals(Tr_level l) {
	return l->formals;
}

static Tr_accessList makeFormalAccessList(Tr_level l) {
 /* get the access-list from frame (ingnore the first one) */
	Tr_accessList head = NULL, tail = NULL;
	F_accessList  acsl = F_formals(l->frame)->tail; /*ignore the first one for static-link*/
	for (; acsl; acsl = acsl->tail) {
		Tr_access ac = Tr_Access(l, acsl->head);
		if (head) {
			tail->tail = Tr_AccessList(ac, NULL);
			tail = tail->tail;
		} else {
			head = Tr_AccessList(ac, NULL);
			tail = head;
		}
	}
	return head;
}

static Tr_level outer = NULL;
Tr_level Tr_outermost(void) {
	if (!outer) outer = Tr_newLevel(NULL, Temp_newlabel(), NULL);
	return outer;
}

static Tr_access Tr_Access(Tr_level l, F_access a) {
	Tr_access T_a = checked_malloc(sizeof(*T_a));
	T_a->level  = l;
	T_a->access = a;
	return T_a;
}

static void display_l(Tr_level l) {
	static int lnum;
	if (l->parent) {
		printf("parent: %s\n", Temp_labelstring(l->parent->name));
	} else {
		printf("parent: root\n");
	}
	printf("addr: %s\n", Temp_labelstring(l->name));
	display_f(l->frame);
}

static void display_ac(Tr_access ac) {
	printf("level: %s\n", Temp_labelstring(ac->level->name));	
	dis_ac(ac->access);
}
