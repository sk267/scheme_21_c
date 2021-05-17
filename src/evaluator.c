#include "scheme.h"

#define EVAL_DEBUG

#ifdef EVAL_DEBUG
#define EVAL_DEBUG_CODE(code) code
#else
#define EVAL_DEBUG_CODE(code)
#endif

scmObject evalFuncOrSyntax(scmObject exprUnevaluated)
{
    scmObject car;
    scmObject restList;
    scmObject nextArg;

    car = exprUnevaluated->value.scmCons.car;
    restList = exprUnevaluated->value.scmCons.cdr;

    EVAL_DEBUG_CODE(
        {
            printf("scm_eval:------------------------- \n");
            printf("restlist: ");
            scm_print(restList);
            printf("car: ");
            scm_print(car);
            printf("\n-----------------\n");
        })

    if (car == newSymbol("+", 1))
    {
        // ADDITION ######################################################
        EVAL_DEBUG_CODE({
            printf("Betrete if + \n");
        })

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
    if (car == newSymbol("-", 1))
    {
        // SUBTRAKTION #####################################################
        EVAL_DEBUG_CODE({
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
    if (car == newSymbol("*", 1))
    {
        // MULTIPLICATION ######################################################
        EVAL_DEBUG_CODE({
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

    if (car == newSymbol("define", 6))
    {
        // DEFINE #########################################

        scmObject key, value;
        key = getCar(restList);

        // getCdr returns Cons, getCar returns first arg of this cons
        // (which is what we are looking for)
        // in case we get an expression here, we want to evaluate it!
        value = scm_eval(getCar(getCdr(restList)));

        setEnvironmentValue(key, value, TOP_ENV);
    }

    if (car == newSymbol("cons", 4))
    {
        // CONS #########################################

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
#
#elif
    return inputToEval;
#endif
}
