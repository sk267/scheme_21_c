#include "scheme.h"

// #define EVAL_SYN_DEBUG

#ifdef EVAL_SYN_DEBUG
#define EVAL_SYN_DEBUG_CODE(code) code
#else
#define EVAL_SYN_DEBUG_CODE(code)
#endif

scmObject DEFINE(int nArgs, scmObject env)
{
    scmObject key, value;

    if (nArgs != 2)
    {
        scmError("define expects exactly 2 arguments");
    }

    value = popFromEvalStack();
    key = popFromEvalStack();

    value = scm_eval(value, env);

    defineEnvironmentValue(key, value, env);
    return SCM_INV;
}

scmObject SET(int nArgs, scmObject env)
{
    scmObject key, value;

    if (nArgs != 2)
    {
        scmError("define expects exactly 2 arguments");
    }

    value = popFromEvalStack();
    key = popFromEvalStack();

    value = scm_eval(value, env);

    setEnvironmentValue(key, value, env);
    return SCM_INV;
}

scmObject DISPLAY(int nArgs, scmObject env)
{
    scmObject currentObject;

    for (int i = 0; i < nArgs; i++)
    {
        currentObject = evalStack[rememberEvalStackPointer + i];

        EVAL_SYN_DEBUG_CODE({
            printf("DISPLAY: currentObject->tag: %d\n", currentObject->tag);
        })
        if ((getTag(currentObject) == TAG_CONS) || (getTag(currentObject) == TAG_SYMBOL))
        {
            currentObject = scm_eval(currentObject, env);
        }
        scm_print(currentObject);
    }

    evalStackPointer = rememberEvalStackPointer;
    return SCM_INV;
}

scmObject QUOTE(int nArgs, scmObject env)
{
    scmError("quote: not yet implemented");
    return SCM_NULL;
}

scmObject IF(int nArgs, scmObject env)
{
    scmObject condUneval, condEvaluated, trueExpr, falseExpr;

    EVAL_SYN_DEBUG_CODE({
        printf("betrete if\n");
    })

    if (nArgs != 3)
    {
        scmError("if expects exactly 3 arguments");
    }

    falseExpr = popFromEvalStack();
    trueExpr = popFromEvalStack();
    condUneval = popFromEvalStack();

    evalStackPointer = rememberEvalStackPointer;

    EVAL_SYN_DEBUG_CODE(
        {
            printf("evalStack[evalStackPointer]: ");
            scm_print(evalStack[evalStackPointer]);
            printf("\n");
        })

    condEvaluated = scm_eval(condUneval, env);
    if (condEvaluated == SCM_FALSE)
    {
        return scm_eval(falseExpr, env);
    }
    else
    {

        return scm_eval(trueExpr, env);
    }
}

static scmObject buildConsFromEvalStack(int idx, int bodyListLength)
{

    scmObject car = (scmObject)malloc(sizeof(struct scmObjectStruct));
    car = evalStack[idx];

    printf("bodyListLength: %d\n", bodyListLength);

    if (idx >= bodyListLength)
    {
        return newCons(car, SCM_NULL);
    }
    else
    {
        return newCons(car, buildConsFromEvalStack(++idx, bodyListLength));
    }
}

scmObject LAMBDA(int nArgs, scmObject env)
{
    scmObject argList, bodyList;
    int bodyListLength = rememberEvalStackPointer + nArgs - 1;

    argList = evalStack[rememberEvalStackPointer];
    bodyList = buildConsFromEvalStack(rememberEvalStackPointer + 1, bodyListLength);

    evalStackPointer = rememberEvalStackPointer;

    return newUserDefinedFunction(argList, bodyList, env);
}

scmObject evalSyntax(scmObject syntaxEvaluated, scmObject restList, scmObject env)
{
    int nArgs;
    int rememberEvalStackPointer;

    rememberEvalStackPointer = evalStackPointer;
    pushListToEvalStack(restList, env);
    nArgs = evalStackPointer - rememberEvalStackPointer;

    return syntaxEvaluated->value.scmSyntax.code(nArgs, env);
}