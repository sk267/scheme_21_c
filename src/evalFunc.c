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
        EVAL_FUNC_CODE({
            printf("Betrete if * \n");
        })

        if (restList->tag == SCM_NULL->tag)
        {
            // Kein Argument eingegeben, leere Liste evaluiert zu 0
            return newInteger(1);
        }

        int fac = 1;
        do
        {
            nextArg = restList->value.scmCons.car;
            fac *= scm_eval(nextArg)->value.scmInt;
            restList = restList->value.scmCons.cdr;
        } while (restList->tag != TAG_NULL);

        return (newInteger(fac));
    }
    case F_TAG_CONS:
    {
        scmObject car, cdr;
        scmObject carEvaluated, cdrEvaluated;

        // getCar has to be evaluated
        // in case we get an expression here, we want to evaluate it!

        car = getCar(restList);
        carEvaluated = scm_eval(car);

        // getCdr returns Cons, getCar returns first arg of this cons
        // (which is what we are looking for)
        // in case we get an expression here, we want to evaluate it!
        cdr = getCdr(restList);
        cdrEvaluated = scm_eval(getCar(cdr));

        if (getCdr(cdr) != SCM_NULL)
        {
            scmError("only two args are allowed for cons!");
        }

        return newCons(carEvaluated, cdrEvaluated);
    }
    case F_TAG_CAR:
    {

        scmObject car;
        scmObject carEvaluated;

        car = getCar(restList);
        carEvaluated = scm_eval(car);

        printf("noch da\n");
        scm_print(carEvaluated);
        printf("\n");

        return getCar(carEvaluated);
    }
    case F_TAG_CDR:
    {

        scmObject cdr;
        scmObject cdrEvaluated;

        cdr = getCar(restList);
        cdrEvaluated = scm_eval(cdr);

        printf("noch da\n");
        scm_print(cdrEvaluated);
        printf("\n");

        return getCdr(cdrEvaluated);
    }

    default:
        break;
    }
    return SCM_NULL;
}