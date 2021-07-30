#include "scheme.h"

#define EVAL_DEBUG

#ifdef EVAL_DEBUG
#define EVAL_DEBUG_CODE(code) code
#else
#define EVAL_DEBUG_CODE(code)
#endif

scmObject evalFuncOrSyntax(scmObject exprUnevaluated, scmObject env)
{
    scmObject funcOrSyn;
    scmObject funcOrSynEvaluated;
    scmObject restList;

    funcOrSyn = exprUnevaluated->value.scmCons.car;
    restList = exprUnevaluated->value.scmCons.cdr;
    funcOrSynEvaluated = scm_eval(funcOrSyn, env);

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
        return evalFunction(funcOrSynEvaluated, restList, env);
        break;

    case TAG_SYN:
        return evalSyntax(funcOrSynEvaluated, restList, env);
        break;

    case TAG_USERDEFINDEFUNC:
        printf("evalFuncOrSyntax: in case TAG_USERDEFINDEFUNC gelandet\n");
        return evalUserDefinedFunction(funcOrSynEvaluated, restList, env);

        break;

    default:
        scmError("got no function or syntax");
        return SCM_NULL;
        break;
    }
    return exprUnevaluated;
}

scmObject scm_eval(scmObject inputToEval, scmObject env)
{

    scmObject evaluated;

    if (inputToEval->tag == TAG_SYMBOL)
    {
        while (env != SCM_NULL)
        {
            // Nach dem Symbol-Value suchen und dieses Binding zurückgeben
            evaluated = getEnvironmentValue(inputToEval, env);
            if (evaluated == SCM_NULL)
            {
                // Variable wurde von getEnvironmentValue nicht gefunden
                // -> Wir suchen im Parent-Env
                env = env->value.scmEnv.parentEnv;
                continue;
            }
            return evaluated;
        }
    }

    if (inputToEval->tag == TAG_CONS)
    {
        return evalFuncOrSyntax(inputToEval, env);
    }

    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    return inputToEval;
}
