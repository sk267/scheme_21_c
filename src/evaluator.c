#include "scheme.h"

scmObject scm_eval(scmObject inputUnevaluated)
{
    scmObject car;
    scmObject restList;
    scmObject nextArg;

    if (inputUnevaluated->tag == TAG_CONS)
    {

        car = inputUnevaluated->value.scmCons.car;
        restList = inputUnevaluated->value.scmCons.cdr;

        printf("scm_eval:------------------------- \n");
        scm_print(restList);
        printf("\n-----------------\n");
        // Wenn es eine Cons ist muss man was machen
        if (car == newSymbol("+", 1))
        {
            printf("Betrete if\n");
            int sum;
            while (restList->tag != TAG_NULL)
            {
                nextArg = restList->value.scmCons.car;
                sum += nextArg->value.scmInt;
                restList = restList->value.scmCons.cdr;
            }
            return (newInteger(sum));
        }
    }
    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    return inputUnevaluated;
}
