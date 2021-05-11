#include "scheme.h"

// #define READER_DEBUG

#ifdef READER_DEBUG
#define READER_DEBUG_CODE(code) code
#else
#define READER_DEBUG_CODE(code)
#endif

scmObject scm_eval(scmObject inputUnevaluated)
{
    scmObject car;
    scmObject restList;
    scmObject nextArg;

    if (inputUnevaluated->tag == TAG_CONS)
    {

        car = inputUnevaluated->value.scmCons.car;
        restList = inputUnevaluated->value.scmCons.cdr;

        READER_DEBUG_CODE(
            {
                printf("scm_eval:------------------------- \n");
                scm_print(restList);
                printf("\n-----------------\n");
            })
        // Wenn es eine Cons ist muss man was machen
        if (car == newSymbol("+", 1))
        {
            READER_DEBUG_CODE({
                printf("Betrete if\n");
            })

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
