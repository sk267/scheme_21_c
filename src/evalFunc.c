#include "scheme.h"

// #define EVAL_FUNC

#ifdef EVAL_FUNC
#define EVAL_FUNC_CODE(code) code
#else
#define EVAL_FUNC_CODE(code)
#endif

scmObject evalFunction(scmObject functionEvaluated, scmObject restList)
{

    scmObject nextArg;
    int nArgs;
    int rememberEvalStackPointer;

    EVAL_FUNC_CODE({
        printf("evalStackPointer vor eval: %d\n", evalStackPointer);
    })
    rememberEvalStackPointer = evalStackPointer;
    EVAL_FUNC_CODE({
        printf("rememberEvalStackPointer: %d", rememberEvalStackPointer);
    })
    evalListAndPushToEvalStack(restList);
    EVAL_FUNC_CODE({
        printf("evalStackPointer nach eval: %d\n", evalStackPointer);
    })
    nArgs = evalStackPointer - rememberEvalStackPointer;
    EVAL_FUNC_CODE({
        printf("nArgs: %d\n", nArgs);
    })

    switch (functionEvaluated->value.scmFunction.whichFunction)
    {
    case F_TAG_PLUS:
    {
        int sum = 0;

        if (nArgs == 0)
        {
            // No Arguments given
            return newInteger(0);
        }
        for (int i = 0; i < nArgs; i++)
        {
            nextArg = popFromEvalStack();
            scmAssert(nextArg->tag == TAG_INT, "got Non-Integer for addition");
            sum += nextArg->value.scmInt;
        }

        return (newInteger(sum));
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
        scmAssert(nextArg->tag == TAG_INT, "got Non-Integer for addition");
        sub = nextArg->value.scmInt;

        for (int i = 1; i < nArgs; i++)
        {
            nextArg = evalStack[rememberEvalStackPointer + i];
            scmAssert(nextArg->tag == TAG_INT, "got Non-Integer for addition");
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
            scmAssert(nextArg->tag == TAG_INT, "got Non-Integer for addition");
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
    return SCM_NULL;
}