#include "scheme.h"

// #define USERDEFINEDFUNCTION_DEBUG

#ifdef USERDEFINEDFUNCTION_DEBUG
#define USERDEFINEDFUNCTION_DEBUG_CODE(code) code
#else
#define USERDEFINEDFUNCTION_DEBUG_CODE(code) //as nothing
#endif

scmObject evalUserDefinedFunction(scmObject lambdaArgsAndBody, scmObject lambdaArgValues, scmObject env)
{

    /*
    Was in scm_eval beispielsweise ankommt:
    scm_eval:------------------------- 

    funcOrSyn: myAdd
    lambdaArgsAndBody:
    lambda: argList: (a SCM_NULL) bodyList: ((+ (a (1 SCM_NULL))) SCM_NULL)

    lambdaArgValues: (22 SCM_NULL)
    -----------------
    */

    USERDEFINEDFUNCTION_DEBUG_CODE(
        {
            printf("\n----------------------------\nbetrete evalUserDefinedFunction: \n");
            printf("lambdaArgsAndBody: ");
            scm_print(lambdaArgsAndBody);
            printf("\nlambdaArgValues:");
            scm_print(lambdaArgValues);
            printf("\n");
        })

    int rememberEvalStackPointer;
    int nArgs;

    scmObject functionsEnv;
    scmObject restInputVars;
    scmObject nextKey;
    scmObject nextValue;

    rememberEvalStackPointer = evalStackPointer;

    evalListAndPushToEvalStack(lambdaArgValues, env);

    nArgs = evalStackPointer - rememberEvalStackPointer;

    // Neues Environment anlegen
    // Dieses braucht als parentEnv das homeEnv des aufrufenden
    scmObject homeEnv = lambdaArgsAndBody->value.scmUserDefindedFunction.homeEnv;
    functionsEnv = allocateEnvironment(7, homeEnv);

    // Alle Argumente (Entsprechende Keys mit den verknüpften Values, die schon evaluiert auf dem Stack liegen)
    // in das tmp-Env reinlegen
    restInputVars = lambdaArgsAndBody->value.scmUserDefindedFunction.argList;
    for (int i = 0; i < nArgs; i++)
    {
        nextKey = getCar(restInputVars);
        nextValue = evalStack[rememberEvalStackPointer + i];

        defineEnvironmentValue(nextKey, nextValue, functionsEnv);
        restInputVars = getCdr(restInputVars);
    }

    // eval-Stack aufräumen
    evalStackPointer = rememberEvalStackPointer;

    // Die body-Liste evaluieren
    scmObject restBodyList = lambdaArgsAndBody->value.scmUserDefindedFunction.bodyList;
    scmObject nextBodyElem;
    scmObject lastValue;

    while (restBodyList != SCM_NULL)
    {

        USERDEFINEDFUNCTION_DEBUG_CODE({
            printf("---in while:\n");
        })

        nextBodyElem = getCar(restBodyList);

        USERDEFINEDFUNCTION_DEBUG_CODE(
            {
                printf("nextBodyElem: ");
                scm_print(nextBodyElem);
                printf("\n");
            })

        lastValue = scm_eval(nextBodyElem, functionsEnv);

        USERDEFINEDFUNCTION_DEBUG_CODE(
            {
                printf("lastValue: ");
                scm_print(lastValue);
                printf("\n");
            })

        restBodyList = getCdr(restBodyList);
    }
    USERDEFINEDFUNCTION_DEBUG_CODE(
        {
            printf("lastValue: ");
            scm_print(lastValue);
            printf("\n");

            printf("\n-----------------------------\n");
        })
    return lastValue;
}