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
    scmObject evaluated;

    if (inputUnevaluated->tag == TAG_SYMBOL)
    {
        // Nach dem Symbol-Value suchen und dieses Binding zurückgeben
        evaluated = getEnvironmentValue(inputUnevaluated, TOP_ENV);
        printf("Betrete errorosodjvfnerjnvg\n");
        if (evaluated == SCM_NULL)
        {
            scmError("variable does not exist yet!");
        }
        return evaluated;
    }

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
