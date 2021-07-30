#include "scheme.h"

#ifdef USERDEFINEDFUNCTION_DEBUG
#define USERDEFINEDFUNCTION_DEBUG_CODE(code) code
#else
#define USERDEFINEDFUNCTION_DEBUG_CODE(code) //as nothing
#endif

scmObject evalUserDefinedFunction(scmObject lambdaArgsAndBody, scmObject lambdaArgValues)
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

    scmObject tmpEnv;
    scmObject restInputVars;
    scmObject nextKey;
    scmObject nextValue;

    rememberEvalStackPointer = evalStackPointer;

    evalListAndPushToEvalStack(lambdaArgValues);

    nArgs = evalStackPointer - rememberEvalStackPointer;

    // Neues Environment anlegen
    tmpEnv = allocateEnvironment(7, TOP_ENV);

    // Alle Argumente (Entsprechende Keys mit den verknüpften Values, die schon evaluiert auf dem Stack liegen)
    // in das tmp-Env reinlegen
    restInputVars = lambdaArgsAndBody->value.scmUserDefindedFunction.argList;
    for (int i = 0; i < nArgs; i++)
    {
        nextKey = getCar(restInputVars);
        nextValue = evalStack[rememberEvalStackPointer + i];

        defineEnvironmentValue(nextKey, nextValue, tmpEnv);
        restInputVars = getCdr(restInputVars);
    }

    // Die body-Liste evaluieren
    scmObject restBodyList = lambdaArgsAndBody->value.scmUserDefindedFunction.bodyList;
    scmObject nextBodyElem = getCar(restBodyList);

    return SCM_NULL;
}