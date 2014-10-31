//@@@
void test_Tr_expList_prepend(Tr_expList l) {
	while(l) {
		printf("Tr_exp->type: %d\n", l->head->kind);
		l = l->tail;
	}
}
//@@@
void test_T_expList(T_expList l) {
	while(l) {
		printf("T_exp->type: %d\n", l->head->kind);
		l = l->tail;
	}
} 
