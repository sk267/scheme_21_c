#include "scheme.h"

// #define EVALSTACK_DEBUG

#ifdef EVALSTACK_DEBUG
#define EVALSTACK_DEBUG_CODE(code) code
#else
#define EVALSTACK_DEBUG_CODE(code)
#endif

#define INITIAL_EVAL_STACK_CAPACITY 50

scmObject *evalStack = NULL;
int evalStackCapacity = 0;
int evalStackPointer = 0;
int rememberEvalStackPointer;

void initializeEvalStack()
{
    evalStack = (scmObject *)malloc(sizeof(scmObject) * INITIAL_EVAL_STACK_CAPACITY);
    evalStackCapacity = INITIAL_EVAL_STACK_CAPACITY;
}

void growEvalStack()
{
    evalStack = (scmObject *)realloc(evalStack, sizeof(scmObject) * evalStackCapacity * 1.8);
    evalStackCapacity = 1.8;
}

void pushToEvalStack(scmObject evaluatedObject)
{
    if (evalStackPointer == evalStackCapacity)
    {
        growEvalStack();
    }
    evalStack[evalStackPointer] = evaluatedObject;
    evalStackPointer++;
    EVALSTACK_DEBUG_CODE({
        printf("pushToEvalStack: pointer: %d\n", evalStackPointer);
    })
}

scmObject popFromEvalStack()
{
    evalStackPointer--;
    return evalStack[evalStackPointer];
}

void evalListAndPushToEvalStack(scmObject restList)
{
    if (restList == SCM_NULL)
    {
        return;
    }
    scmObject nextUnevaluatedArg;
    do
    {
        nextUnevaluatedArg = getCar(restList);
        EVALSTACK_DEBUG_CODE(
            {
                printf("evalListAndPushToEvalStack: going to be pushed: \n");
                scm_print(scm_eval(nextUnevaluatedArg));
                printf(" \n");
            })
        pushToEvalStack(scm_eval(nextUnevaluatedArg));
        restList = getCdr(restList);
    } while (restList->tag != TAG_NULL);
}