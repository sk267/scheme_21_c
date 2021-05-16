#include "scheme.h"

#define EVAL_DEBUG

#ifdef EVAL_DEBUG
#define EVAL_DEBUG_CODE(code) code
#else
#define EVAL_DEBUG_CODE(code)
#endif

scmObject evalFuncOrSyntax(scmObject exprUnevaluated)
{
    scmObject car;
    scmObject restList;
    scmObject nextArg;

    car = exprUnevaluated->value.scmCons.car;
    restList = exprUnevaluated->value.scmCons.cdr;

    EVAL_DEBUG_CODE(
        {
            printf("scm_eval:------------------------- \n");
            printf("restlist: ");
            scm_print(restList);
            printf("\n-----------------\n");
        })

    if (car == newSymbol("+", 1))
    {
        EVAL_DEBUG_CODE({
            printf("Betrete if + \n");
        })

        int sum = 0;

        do
        {
            nextArg = restList;
            printf("noch da\n");
            printf("%d\n", nextArg->value.scmInt);
            sum += nextArg->value.scmInt;
            printf("evalFuncOrSyntax: sum: %d\n", sum);
            restList = restList->value.scmCons.cdr;
        } while (restList->tag != TAG_NULL);

        return (newInteger(sum));
    }

    return exprUnevaluated;
}

scmObject scm_eval(scmObject inputUnevaluated)
{

    scmObject evaluated;

    if (inputUnevaluated->tag == TAG_SYMBOL)
    {
        // Nach dem Symbol-Value suchen und dieses Binding zurückgeben
        evaluated = getEnvironmentValue(inputUnevaluated, TOP_ENV);
        if (evaluated == SCM_NULL)
        {
            scmError("variable does not exist yet!");
        }
        return evaluated;
    }

    if (inputUnevaluated->tag == TAG_CONS)
    {
        return evalFuncOrSyntax(inputUnevaluated);
    }
    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    return inputUnevaluated;
}
