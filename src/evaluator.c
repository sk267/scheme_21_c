#include "scheme.h"

// #define EVAL_DEBUG

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
        EVAL_DEBUG_CODE({
            printf("evalFuncOrSyntax: in case TAG_USERDEFINDEFUNC gelandet\n");
        })
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

    if (getTag(inputToEval) == TAG_SYMBOL)
    {
        return getEnvironmentValue(inputToEval, env);
    }

    if (getTag(inputToEval) == TAG_CONS)
    {
        return evalFuncOrSyntax(inputToEval, env);
    }

    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    return inputToEval;
}

// /////////////////////////////////////////////////////////
// Trampoline Code
// /////////////////////////////////////////////////////////

/*
caller:
    pushToEvalStack(inputToEval)
    pushToEvalStack(env)
    trampoline(C_scm_eval)
*/
contFunc C_scm_eval()
{
    // expects on evalStack: inputToEval, env

    scmObject retVal;
    scmObject inputToEval, env;

    env = popFromEvalStack();
    inputToEval = popFromEvalStack();

    if (getTag(inputToEval) == TAG_SYMBOL)
    {
        // Push kann man sich sparen, wenn man es oben einfach weglässt
        pushToEvalStack(inputToEval);
        pushToEvalStack(env);
        return C_getEnvironmentValue;
    }

    if (getTag(inputToEval) == TAG_CONS)
    {
        // Push kann man sich sparen, wenn man es oben einfach weglässt
        pushToEvalStack(inputToEval);
        pushToEvalStack(env);
        return C_evalFuncOrSyntax;
    }

    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)
    // return inputToEval;          ---             old Code
    retVal = inputToEval;
    return rPOP();
}

scmObject retVal;
contFunc *returnStack = NULL;
int returnStackCapacity = 0;
int returnStackPointer = 0;

void initializeReturnStack()
{
    returnStack = (contFunc *)malloc(sizeof(contFunc) * INITIAL_RETURN_STACK_SIZE);
    returnStackCapacity = INITIAL_RETURN_STACK_SIZE;
    returnStackPointer = 0;
}

void growReturnStack()
{
    int oldCapacity = returnStackCapacity;
    int newCapacity = oldCapacity * 5 / 4;
    returnStack = (contFunc *)realloc(returnStack, sizeof(contFunc) * newCapacity);
    returnStackCapacity = newCapacity;
}

void rPUSH(contFunc obj)
{
    returnStack[returnStackPointer] = obj;
    returnStackPointer++;
    if (returnStackPointer == returnStackCapacity)
    {
        growReturnStack();
    }
}

contFunc rPOP()
{
    return returnStack[--returnStackPointer];
}

void trampoline(contFunc startingPoint)
{
    contFunc nextCont = startingPoint;

    rPUSH(NULL);
    while (nextCont != NULL)
    {
        nextCont = (*nextCont)();
    }
}
