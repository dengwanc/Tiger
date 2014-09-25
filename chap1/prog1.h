#define MAX_QUEUE 500

struct queone {
	enum {A_stm_e, A_exp_e, A_expList_e, A_null} kind; 
	union {
		A_stm stm;
		A_exp exp;
		A_expList explist;
	} p;
};

A_stm prog(void);
A_stm prog1(void);
A_stm prog2(void);
int maxargs(A_stm s);

