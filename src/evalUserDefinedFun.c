#include "scheme.h"

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
    functionsEnv = allocateEnvironment(7, TOP_ENV);

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
        nextBodyElem = getCar(restBodyList);
        lastValue = scm_eval(nextBodyElem, functionsEnv);

        restBodyList = getCdr(restBodyList);
    }

    return lastValue;
}