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

    switch (functionEvaluated->value.scmFunction.whichFunction)
    {
    case F_TAG_PLUS:
    {
        int sum = 0;

        if (restList->tag == SCM_NULL->tag)
        {
            // Kein Argument eingegeben, leere Liste evaluiert zu 0
            return newInteger(0);
        }
        do
        {
            nextArg = getCar(restList);
            scmAssert(scm_eval(nextArg)->tag == TAG_INT, "got non Int value for addition!");
            sum += scm_eval(nextArg)->value.scmInt;
            restList = getCdr(restList);
        } while (restList->tag != TAG_NULL);

        return (newInteger(sum));
    }
    case F_TAG_MINUS:
    {
        // SUBTRAKTION #####################################################
        EVAL_FUNC_CODE({
            printf("Betrete if + \n");
        })

        if (restList->tag == SCM_NULL->tag)
        {
            // Kein Argument eingegeben, leere Liste evaluiert zu 0
            return newInteger(0);
        }

        nextArg = restList->value.scmCons.car;
        int sub = scm_eval(nextArg)->value.scmInt;
        restList = restList->value.scmCons.cdr;

        while (restList->tag != TAG_NULL)
        {
            nextArg = restList->value.scmCons.car;
            sub -= scm_eval(nextArg)->value.scmInt;
            restList = restList->value.scmCons.cdr;
        };

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