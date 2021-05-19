#include "scheme.h"

// #define EVAL_SYN_DEBUG

#ifdef EVAL_SYN_DEBUG
#define EVAL_SYN_DEBUG_CODE(code) code
#else
#define EVAL_SYN_DEBUG_CODE(code)
#endif

scmObject evalSyntax(scmObject syntaxEvaluated, scmObject restList)
{

    int nArgs;
    int rememberEvalStackPointer;

    rememberEvalStackPointer = evalStackPointer;
    pushListToEvalStack(restList);
    nArgs = evalStackPointer - rememberEvalStackPointer;

    switch (syntaxEvaluated->value.scmSyntax.whichSyntax)
    {
    case S_TAG_DEFINE:
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
    case S_TAG_SET:
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
    case S_TAG_DISPLAY:
    {
        for (int i = 0; i < nArgs; i++)
        {
            scm_print(evalStack[rememberEvalStackPointer + i]);
        }
        return SCM_INV;
    }
    case S_TAG_QUOTE:
    {
        scmError("quote: not yet implemented");
    }
    case S_TAG_IF:
    {
        scmObject condUneval, exprToEvaluate;
        scmObject condEvaluated;

        if (nArgs != 3)
        {
            scmError("if expects exactly 3 arguments");
        }

        condUneval = evalStack[rememberEvalStackPointer];
        condEvaluated = scm_eval(condUneval);

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
        return scm_eval(exprToEvaluate);
    }

    default:
        return SCM_NULL;
        break;
    }
}