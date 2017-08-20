#ifndef TIGER_SEMANTIC_H
#define TIGER_SEMANTIC_H

#include "../utils/binary-tree.h"

namespace ast {
    class SemanticResult {
    public:
        BinaryTree* val_table;
        BinaryTree* typ_table;
        ActualType* type;
        SemanticResult(BinaryTree* v, BinaryTree* t, ActualType* a);
        SemanticResult* copy(ActualType* t);
        SemanticResult* copy();
    };
}

ast::SemanticResult* makeBaseEnvTable();

#endif