#include "scheme.h"

#define EVAL_DEBUG

#ifdef EVAL_DEBUG
#define EVAL_DEBUG_CODE(code) code
#else
#define EVAL_DEBUG_CODE(code)
#endif

scmObject evalFuncOrSyntax(scmObject exprUnevaluated)
{
    scmObject funcOrSyn;
    scmObject funcOrSynEvaluated;
    scmObject restList;

    funcOrSyn = exprUnevaluated->value.scmCons.car;
    restList = exprUnevaluated->value.scmCons.cdr;
    funcOrSynEvaluated = scm_eval(funcOrSyn);

    EVAL_DEBUG_CODE(
        {
            printf("scm_eval:------------------------- \n");
            printf("restlist: ");
            scm_print(restList);
            printf("\n funcOrSyn: ");
            scm_print(funcOrSyn);
            printf("\n");
            printf("funcOrSynEvaluated: \n");
            scm_print(funcOrSynEvaluated);
            printf("\n-----------------\n");
        })

    switch (funcOrSynEvaluated->tag)
    {
    case TAG_FUNC:
        return evalFunction(funcOrSynEvaluated, restList);
        break;

    case TAG_SYN:
        return evalSyntax(funcOrSynEvaluated, restList);
        break;

    case TAG_USERDEFINDEFUNC:
        printf("evalFuncOrSyntax: in case TAG_USERDEFINDEFUNC gelandet\n");
        return evalUserDefinedFunction(funcOrSynEvaluated, restList);

        break;

    default:
        scmError("got no function or syntax");
        return SCM_NULL;
        break;
    }
    return exprUnevaluated;
}

scmObject scm_eval(scmObject inputToEval)
{

#ifdef true
    scmObject evaluated;

    if (inputToEval->tag == TAG_SYMBOL)
    {
        // Nach dem Symbol-Value suchen und dieses Binding zurückgeben
        evaluated = getEnvironmentValue(inputToEval, TOP_ENV);
        if (evaluated == SCM_NULL)
        {
            scmError("variable does not exist yet!");
        }
        return evaluated;
    }

    if (inputToEval->tag == TAG_CONS)
    {
        return evalFuncOrSyntax(inputToEval);
    }
    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    return inputToEval;

#elif
    return inputToEval;
#endif
}
