#include "scheme.h"

// #define EVAL_SYN_DEBUG

#ifdef EVAL_SYN_DEBUG
#define EVAL_SYN_DEBUG_CODE(code) code
#else
#define EVAL_SYN_DEBUG_CODE(code)
#endif

scmObject DEFINE(int nArgs)
{
    scmObject key, value;

    if (nArgs != 2)
    {
        scmError("define expects exactly 2 arguments");
    }

    value = popFromEvalStack();
    key = popFromEvalStack();

    value = scm_eval(value);

    defineEnvironmentValue(key, value, TOP_ENV);
    return SCM_INV;
}

scmObject SET(int nArgs)
{
    scmObject key, value;

    if (nArgs != 2)
    {
        scmError("define expects exactly 2 arguments");
    }

    value = popFromEvalStack();
    key = popFromEvalStack();

    value = scm_eval(value);

    setEnvironmentValue(key, value, TOP_ENV);
    return SCM_INV;
}

scmObject DISPLAY(int nArgs)
{
    for (int i = 0; i < nArgs; i++)
    {
        scm_print(evalStack[rememberEvalStackPointer + i]);
    }
    return SCM_INV;
}

scmObject QUOTE(int nArgs)
{
    scmError("quote: not yet implemented");
    return SCM_NULL;
}

scmObject IF(int nArgs)
{
    scmObject condUneval, exprToEvaluate, condEvaluated;

    if (nArgs != 3)
    {
        scmError("if expects exactly 3 arguments");
    }

    condUneval = evalStack[rememberEvalStackPointer];
    condEvaluated = scm_eval(condUneval);

    EVAL_SYN_DEBUG_CODE(
        {
            printf("IF: condEvaluated: ");
            scm_print(condEvaluated);
            printf("\n");
        })

    if (condEvaluated == SCM_TRUE)
    {
        exprToEvaluate = evalStack[rememberEvalStackPointer + 1];
    }
    else if (condEvaluated == SCM_FALSE)
    {
        exprToEvaluate = evalStack[rememberEvalStackPointer + 2];
    }
    else
    {
        scmError("if condition did not evaluat to #t or #f");
    }
    evalStackPointer = rememberEvalStackPointer;
    return scm_eval(exprToEvaluate);
}

scmObject evalSyntax(scmObject syntaxEvaluated, scmObject restList)
{
    int nArgs;
    int rememberEvalStackPointer;

    rememberEvalStackPointer = evalStackPointer;
    pushListToEvalStack(restList);
    nArgs = evalStackPointer - rememberEvalStackPointer;

    return syntaxEvaluated->value.scmSyntax.code(nArgs);
}