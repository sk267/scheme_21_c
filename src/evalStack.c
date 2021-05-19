#include "scheme.h"

#define INITIAL_EVAL_STACK_CAPACITY 10

scmObject *evalStack = NULL;
int evalStackCapacity = 0;
int evalStackPointer = 0;

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
}

scmObject popFromEvalStack()
{
    evalStackPointer--;
    return evalStack[evalStackPointer];
}
