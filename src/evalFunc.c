#include "scheme.h"

// #define EVAL_FUNC

#ifdef EVAL_FUNC
#define EVAL_FUNC_CODE(code) code
#else
#define EVAL_FUNC_CODE(code)
#endif

scmObject PLUS(int nArgs)
{

    // printf("Betrete PLUS\n");
    scmObject nextArg;
    int sum = 0;

    if (nArgs == 0)
    {
        // No Arguments given
        return newInteger(0);
    }
    for (int i = 0; i < nArgs; i++)
    {
        nextArg = popFromEvalStack();
        scmAssert((isInteger(nextArg)), "got Non-Integer for addition");
        sum += getIntVal(nextArg);
    }

    return (newInteger(sum));
}

scmObject MINUS(int nArgs)
{
    int sub;
    scmObject nextArg;

    if (nArgs == 0)
    {
        // No Arguments given
        return newInteger(0);
    }

    nextArg = evalStack[rememberEvalStackPointer];
    scmAssert(isInteger(nextArg), "got Non-Integer for addition");
    sub = getIntVal(nextArg);

    for (int i = 1; i < nArgs; i++)
    {
        nextArg = evalStack[rememberEvalStackPointer + i];
        scmAssert(isInteger(nextArg), "got Non-Integer for addition");
        sub -= getIntVal(nextArg);
    }
    evalStackPointer = rememberEvalStackPointer;
    return (newInteger(sub));
}

scmObject MULT(int nArgs)
{
    scmObject nextArg;
    int fac = 1;

    if (nArgs == 0)
    {
        // No Arguments given
        return newInteger(1);
    }
    for (int i = 0; i < nArgs; i++)
    {
        nextArg = popFromEvalStack();
        scmAssert(isInteger(nextArg), "got Non-Integer for addition");
        fac *= getIntVal(nextArg);
    }

    return (newInteger(fac));
}

scmObject CONS(int nArgs)
{
    scmObject car, cdr;

    if (nArgs != 2)
    {
        evalStackPointer = rememberEvalStackPointer;
        scmError("Cons expects exactly 2 arguments!");
    }

    cdr = popFromEvalStack();
    car = popFromEvalStack();

    return newCons(car, cdr);
}

scmObject CAR(int nArgs)
{
    scmObject cons;

    if (nArgs != 1)
    {
        evalStackPointer = rememberEvalStackPointer;
        scmError("Cons expects exactly 1 argument!");
    }

    cons = popFromEvalStack();
    return getCar(cons);
}

scmObject CDR(int nArgs)
{
    scmObject cons;

    if (nArgs != 1)
    {
        evalStackPointer = rememberEvalStackPointer;
        scmError("Cons expects exactly 1 argument!");
    }

    cons = popFromEvalStack();
    return getCdr(cons);
}

scmObject EQ(int nArgs)
{
    scmObject firstArg, secondArg;

    firstArg = evalStack[rememberEvalStackPointer];
    secondArg = evalStack[rememberEvalStackPointer + 1];

    evalStackPointer = rememberEvalStackPointer;

    if (nArgs != 2)
    {
        scmError("EQ expects exactly 2 arguments");
    }

    if (firstArg == secondArg)
    {
        return SCM_TRUE;
    }
    else
    {
        return SCM_FALSE;
    }
}

scmObject EQNR(int nArgs)
{
    // printf("betrete EQNR");
    scmObject firstArg, secondArg;

    firstArg = evalStack[rememberEvalStackPointer];
    secondArg = evalStack[rememberEvalStackPointer + 1];
    evalStackPointer = rememberEvalStackPointer;

    if (nArgs != 2)
    {
        scmError("EQ expects exactly 2 arguments");
    }

    if (!isInteger(firstArg) || !isInteger(secondArg))
    {
        scmError("= needs two int values!");
    }

    if (getIntVal(firstArg) == getIntVal(secondArg))
    {
        return SCM_TRUE;
    }
    else
    {
        return SCM_FALSE;
    }
}

scmObject GTNR(int nArgs)
{

    scmObject firstArg, secondArg;

    firstArg = evalStack[rememberEvalStackPointer];
    secondArg = evalStack[rememberEvalStackPointer + 1];

    evalStackPointer = rememberEvalStackPointer;

    if (nArgs != 2)
    {
        scmError("> expects exactly 2 arguments");
    }

    if (!isInteger(firstArg) || !isInteger(secondArg))
    {
        scmError("> needs two int values!");
    }

    if (getIntVal(firstArg) > getIntVal(secondArg))
    {
        return SCM_TRUE;
    }
    else
    {
        return SCM_FALSE;
    }
}

scmObject SMNR(int nArgs)
{

    scmObject firstArg, secondArg;

    firstArg = evalStack[rememberEvalStackPointer];
    secondArg = evalStack[rememberEvalStackPointer + 1];

    evalStackPointer = rememberEvalStackPointer;

    if (nArgs != 2)
    {
        scmError("< expects exactly 2 arguments");
    }

    if (!isInteger(firstArg) || !isInteger(secondArg))
    {
        scmError("< needs two int values!");
    }

    if (getIntVal(firstArg) < getIntVal(secondArg))
    {
        return SCM_TRUE;
    }
    else
    {
        return SCM_FALSE;
    }
}

scmObject EVAL(int nArgs)
{
    scmObject objectToEvaluate;
    if (nArgs != 1)
    {
        evalStackPointer = rememberEvalStackPointer;
        scmError("Eval expects exactly 1 argument!");
    }

    objectToEvaluate = popFromEvalStack();
    return objectToEvaluate;
}

scmObject evalFunction(scmObject functionEvaluated, scmObject restList, scmObject env)
{
    int nArgs;
    int rememberEvalStackPointer;

    EVAL_FUNC_CODE({
        printf("evalStackPointer vor eval: %d\n", evalStackPointer);
    })

    rememberEvalStackPointer = evalStackPointer;

    EVAL_FUNC_CODE({
        printf("rememberEvalStackPointer: %d", rememberEvalStackPointer);
    })
    evalListAndPushToEvalStack(restList, env);

    EVAL_FUNC_CODE({
        printf("evalStackPointer nach eval: %d\n", evalStackPointer);
    })

    nArgs = evalStackPointer - rememberEvalStackPointer;
    EVAL_FUNC_CODE({
        printf("nArgs: %d\n", nArgs);
    })

    return functionEvaluated->value.scmFunction.code(nArgs);

#ifdef asd

    switch (functionEvaluated->value.scmFunction.whichFunction)
    {
    case F_TAG_PLUS:
    {
        printf("should not go in here!\n");
        // int sum = 0;

        // if (nArgs == 0)
        // {
        //     // No Arguments given
        //     return newInteger(0);
        // }
        // for (int i = 0; i < nArgs; i++)
        // {
        //     nextArg = popFromEvalStack();
        //     scmAssert(getTag(nextArg) == TAG_INT, "got Non-Integer for addition");
        //     sum += nextArg->value.scmInt;
        // }

        // return (newInteger(sum));
    }
    case F_TAG_MINUS:
    {
        int sub;

        if (nArgs == 0)
        {
            // No Arguments given
            return newInteger(0);
        }

        nextArg = evalStack[rememberEvalStackPointer];
        scmAssert(isInteger(getTag(nextArg)), "got Non-Integer for addition");
        sub = nextArg->value.scmInt;

        for (int i = 1; i < nArgs; i++)
        {
            nextArg = evalStack[rememberEvalStackPointer + i];
            scmAssert(isInteger(getTag(nextArg)), "got Non-Integer for addition");
            sub -= nextArg->value.scmInt;
        }
        evalStackPointer = rememberEvalStackPointer;
        return (newInteger(sub));
    }

    case F_TAG_MULT:
    {

        int fac = 1;

        if (nArgs == 0)
        {
            // No Arguments given
            return newInteger(1);
        }
        for (int i = 0; i < nArgs; i++)
        {
            nextArg = popFromEvalStack();
            scmAssert(getTag(nextArg) == TAG_INT, "got Non-Integer for addition");
            fac *= nextArg->value.scmInt;
        }

        return (newInteger(fac));
    }
    case F_TAG_CONS:
    {
        scmObject car, cdr;

        if (nArgs != 2)
        {
            scmError("Cons expects exactly 2 arguments!");
        }

        cdr = popFromEvalStack();
        car = popFromEvalStack();

        return newCons(car, cdr);
    }
    case F_TAG_CAR:
    {
        scmObject cons;

        if (nArgs != 1)
        {
            scmError("Cons expects exactly 1 argument!");
        }

        cons = popFromEvalStack();
        return getCar(cons);
    }
    case F_TAG_CDR:
    {
        scmObject cons;

        if (nArgs != 1)
        {
            scmError("Cons expects exactly 1 argument!");
        }

        cons = popFromEvalStack();
        return getCdr(cons);
    }
    case F_TAG_EVAL:
    {
        scmObject objectToEvaluate;
        if (nArgs != 1)
        {
            scmError("Eval expects exactly 1 argument!");
        }

        objectToEvaluate = popFromEvalStack();
        return objectToEvaluate;
    }

    default:
        break;
    }

#endif
    return SCM_NULL;
}
