#include "scheme.h"

// #define EVAL_SYN_DEBUG

#ifdef EVAL_SYN_DEBUG
#define EVAL_SYN_DEBUG_CODE(code) code
#else
#define EVAL_SYN_DEBUG_CODE(code)
#endif

scmObject evalSyntax(scmObject syntaxEvaluated, scmObject restList)
{
    switch (syntaxEvaluated->value.scmSyntax.whichSyntax)
    {
    case S_TAG_DEFINE:
    {
        scmObject key, value;
        key = getCar(restList);

        // getCdr returns Cons, getCar returns first arg of this cons
        // (which is what we are looking for)
        // in case we get an expression here, we want to evaluate it!
        value = scm_eval(getCar(getCdr(restList)));

        defineEnvironmentValue(key, value, TOP_ENV);
        return SCM_INV;
    }
    case S_TAG_SET:
    {
        scmObject keyUneval, valueUneval;
        scmObject valueEvaluated;

        keyUneval = getCar(restList);
        restList = getCdr(restList);

        valueUneval = getCar(restList);
        valueEvaluated = scm_eval(valueUneval);

        setEnvironmentValue(keyUneval, valueEvaluated, TOP_ENV);
        return SCM_INV;
    }
    case S_TAG_DISPLAY:
    {
        // DISPLAY #########################################

        scmObject carUneval;
        scmObject carEvaluated;

        carUneval = getCar(restList);
        carEvaluated = scm_eval(carUneval);
        scm_print(carEvaluated);
        return SCM_INV;
    }
    case S_TAG_QUOTE:
    {
        scmObject carUneval;

        carUneval = getCar(restList);
        scm_print(carUneval);
        return SCM_INV;
    }
    case S_TAG_IF:
    {
        // IF #########################################

        EVAL_SYN_DEBUG_CODE({
            printf("betrete if\n");
        })
        scmObject condUneval, trueExprUneval, falseExprUneval;
        scmObject condEvaluated;

        condUneval = getCar(restList);
        restList = getCdr(restList);

        EVAL_SYN_DEBUG_CODE(
            {
                printf("condUneval: ");
                scm_print(condUneval);
                printf("\n");
            })

        trueExprUneval = getCar(restList);
        restList = getCdr(restList);

        EVAL_SYN_DEBUG_CODE(
            {
                printf("trueExprUneval: ");
                scm_print(trueExprUneval);
                printf("\n");
            })

        falseExprUneval = getCar(restList);

        EVAL_SYN_DEBUG_CODE(
            {
                printf("falseExprUneval: ");
                scm_print(falseExprUneval);
                printf("\n");
            })

        condEvaluated = scm_eval(condUneval);

        if (condEvaluated == SCM_TRUE)
        {
            return scm_eval(trueExprUneval);
        }
        else if (condEvaluated == SCM_FALSE)
        {
            return scm_eval(falseExprUneval);
        }
        else
        {
            return SCM_NULL;
        }
    }

    default:
        return SCM_NULL;
        break;
    }
}