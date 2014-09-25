#include "util.h"
#include "slp.h"
#include "prog1.h"
#define ADD_QUE(ind, kindval, pty, pval) queue[ind].kind = kindval;\
										 queue[ind].p.pty = pval;  \
										 ind++
/*
** s: a := 5+3; b := print (a, (a-1), 10*a); print (b)
 */
A_stm prog(void) 
{
    return 
    A_CompoundStm(
	    A_AssignStm("a",A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),   
		A_CompoundStm(
			A_AssignStm(
				"b",
				A_EseqExp(
					A_PrintStm(
						A_PairExpList(
							A_IdExp("a"),
							A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, A_NumExp(1)))
						)
					),
					A_OpExp(A_NumExp(10), A_times, A_IdExp("a"))
			    )
			),
			A_PrintStm(A_LastExpList(A_IdExp("b")))
		)
   );
}
/* print (3,4,5,4) */
A_stm prog1(void) 
{
	return 
	A_PrintStm(
		A_PairExpList(
			A_NumExp(3),
			A_PairExpList(
				A_NumExp(4),
				A_PairExpList(
					A_NumExp(5),
					A_LastExpList(A_NumExp(4))
				)
			)
		)
	);
}
/* print (1,2,3,(print(1,2,3,4), 6)) */
A_stm prog2(void)
{
	return
	A_PrintStm(
		A_PairExpList(
			A_NumExp(1),
			A_PairExpList(
				A_NumExp(2),
				A_PairExpList(
					A_NumExp(3),
					A_LastExpList(
						A_EseqExp(
							A_PrintStm(
								A_PairExpList(
									A_NumExp(1),
									A_PairExpList(
										A_NumExp(2),
										A_PairExpList(
											A_NumExp(3),
											A_LastExpList(A_NumExp(4))
										)
									)
								)
							),
							A_NumExp(6)
						)
					)
				)
			)
		)
	);
}
int maxargs(A_stm s)
{
	/*init*/
	int temp_max = 0; 
	int max = 0;
	int i; /*current position*/
	int j; /*queue position*/
	struct queone queue[MAX_QUEUE];
	for (i = 0; i < MAX_QUEUE; i++) {
		queue[i].kind = A_null;
	}
    j = 0;

	/* add init data to queue */
	ADD_QUE(j, A_stm_e, stm, s);
	/*  */
	for (i = 0; (queue[i].kind != A_null); i++) {
		if (queue[i].kind == A_stm_e) {
			switch (queue[i].p.stm->kind){
			case A_printStm:
				temp_max = 0;
				/* add pair-exp to queue */
				A_expList temp = queue[i].p.stm->u.print.exps;
				ADD_QUE(j, A_expList_e, explist, temp);
				/* bad code ex: !!! equals ADD_QUE(ind, ...)
				queue[j].kind = A_expList_e;
				queue[j].p.explist = queue[i].p.stm->u.print.exps;
				j++;
				*/

				/* count print args */
                while (temp->kind != A_lastExpList)	{
					temp_max++;
					temp = temp->u.pair.tail;
				}
				if ((++temp_max) > max) {
					max = temp_max;
				}
				/*bingo*/
				break;
			case A_compoundStm:
				ADD_QUE(j, A_stm_e, stm, queue[i].p.stm->u.compound.stm1);
				ADD_QUE(j, A_stm_e, stm, queue[i].p.stm->u.compound.stm2);
				break;
			case A_assignStm:
				ADD_QUE(j, A_exp_e, exp, queue[i].p.stm->u.assign.exp);
				break;
			}
		}
		if (queue[i].kind == A_exp_e) {
			switch (queue[i].p.exp->kind){
			case A_opExp:
				ADD_QUE(j, A_exp_e, exp, queue[i].p.exp->u.op.left);
				ADD_QUE(j, A_exp_e, exp, queue[i].p.exp->u.op.right);
				break;
			case A_eseqExp:
				ADD_QUE(j, A_stm_e, stm, queue[i].p.exp->u.eseq.stm);
				ADD_QUE(j, A_exp_e, exp, queue[i].p.exp->u.eseq.exp);
				break;
			}
		}
		if (queue[i].kind == A_expList_e) {
			switch (queue[i].p.explist->kind){
			case A_pairExpList:
				ADD_QUE(j, A_exp_e, exp, queue[i].p.explist->u.pair.head);
				ADD_QUE(j, A_expList_e, explist, queue[i].p.explist->u.pair.tail);
				break;
			case A_lastExpList:
				ADD_QUE(j, A_exp_e, exp, queue[i].p.explist->u.last);
				break;
			default:
				/* do something */
				break;
			}
		}
	}
	/* look a queue ele kind */
	/*
	for (i = 0; i < MAX_QUEUE && (queue[i].kind != A_null); i++) {
			printf("%d\n", queue[i].kind);
	}
	*/
	return max;
}

/*
** you'd better add () after sizeof, fuck...
** not try to recorgnize data-type, the only-way: extra-record
** 
** how to test ??
** why so low use c !! 
 */
