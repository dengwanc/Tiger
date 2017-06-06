#include "interpreter.h"
#include <cassert>
#include <vector>
/**
 * a := 5 + 3; b := (print (a, a-1), 10*a); print (b);
 */
Statement* makeSentence1()
{
    // part 1
    auto _5p3= new Expression(
        new Expression(5),
        plus,
        new Expression(3)
    );
    auto _a = new Statement("a", _5p3);

    // part 2
    auto* _am1 = new Expression(
        new Expression("a"),
        minus,
        new Expression(1)
    );
    auto* _exprs = new ExprList(
        new Expression("a"),
        new ExprList(_am1)
    );
    auto* _10ta = new Expression(
        new Expression(10),
        times,
        new Expression("a")
    );
    auto* _print1 = new Statement(_exprs);
    auto* _eseq = new Expression(_print1, _10ta);
    auto* _b = new Statement("b", _eseq);

    // part 3
    auto* _print_b = new Statement(
        new ExprList(new Expression("b"))
    );

    // merge
    return new Statement(
        _a,
        new Statement(_b, _print_b)
    );
}

/**
 * print(1, 2, 4)
 */
Statement* makeSentence2()
{
    auto* _4 = new ExprList(new Expression(4));
    auto* _2_4 = new ExprList(new Expression(2), _4);
    auto* _1_2_4 = new ExprList(new Expression(1), _2_4);

    return new Statement(_1_2_4);
}

int main()
{
    // test and verify maxargs
    // use queue Breadth traversal
    // the AST
    auto s1 = makeSentence1();
    auto s2 = makeSentence2();
    assert(s1->maxargs() == 2);
    assert(s2->maxargs() == 3);

    s1->interpret();
    s2->interpret();
}