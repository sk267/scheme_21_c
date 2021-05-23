#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>

#define scmAssert(ok, msg) scmAssertVerbose(ok, msg, __FILE__, __LINE__)
#define scmError(msg) scmErrorVerbose(msg, __FILE__, __LINE__)

enum
{
    TAG_INT = 1,
    TAG_DOUBLE,
    TAG_STRING,
    TAG_SYMBOL,
    TAG_CONS,
    TAG_TRUE,
    TAG_FALSE,
    TAG_NULL,
    TAG_INV,
    TAG_ENV,
    TAG_FUNC,
    TAG_SYN
};

enum
{
    F_TAG_PLUS = 1,
    F_TAG_MINUS,
    F_TAG_MULT,
    F_TAG_CONS,
    F_TAG_CAR,
    F_TAG_CDR,
    F_TAG_EVAL,

};

enum
{
    S_TAG_DEFINE = 1,
    S_TAG_QUOTE,
    S_TAG_SET,
    S_TAG_DISPLAY,
    S_TAG_IF,
};

typedef struct scmObjectStruct *scmObject;
typedef struct consStruct cons;
typedef struct envStruct envStr;
typedef struct funcStruct scmFunc;
typedef struct synStruct scmSyn;
typedef scmObject (*scmObjectFunctionPointer)();

struct consStruct
{
    scmObject car;
    scmObject cdr;
};

struct envStruct
{
    scmObject *keyValuePairs;
    int capacity;
    int nVariables;
};

struct funcStruct
{
    int whichFunction;
    scmObjectFunctionPointer code;
};

struct synStruct
{
    int whichSyntax;
};

struct scmObjectStruct
{
    int tag;

    union scmObjectUnion
    {
        int scmInt;
        char *scmChar;
        char *scmSymbol;
        cons scmCons;
        envStr scmEnv;
        double scmDouble;
        scmFunc scmFunction;
        scmSyn scmSyntax;
    } value;
};

extern scmObject *existingSymbols;

extern void selftest();
extern int nextChar();
extern void unreadChar(char input);

extern scmObject newInteger(int inInt);
extern scmObject newString(char *input, int length);
extern scmObject newSymbol(char *input, int length);
extern scmObject newSymbolAllocation(char *input, int length);
extern scmObject newCons(scmObject car, scmObject cdr);
extern scmObject newFunc(scmObjectFunctionPointer code);
extern scmObject newSyntax(int whichFunction);

extern scmObject allocateEnvironment(int inCapacitiy);
extern void initializeTopEnv();

extern void initializeFunctions();
extern void initializeSyntax();

extern void defineEnvironmentValue(scmObject key, scmObject value, scmObject env);
extern void setEnvironmentValue(scmObject key, scmObject value, scmObject env);
extern scmObject getEnvironmentValue(scmObject key, scmObject env);
extern int hashForEnv(scmObject symbol, scmObject env);
extern scmObject evalFuncOrSyntax(scmObject exprUnevaluated);
extern scmObject evalFunction(scmObject functionEvaluated, scmObject restList);
extern scmObject evalSyntax(scmObject functionEvaluated, scmObject restList);

extern void scmAssertVerbose(int ok, char *msg, char *file, int line);
extern void scmErrorVerbose(char *msg, char *file, int line);

extern void initializeSymbolTableBuffer();

extern scmObject scm_read();
extern scmObject scm_eval(scmObject input);
extern void scm_print(scmObject input);

extern scmObject getCar(scmObject hopefullyCons);
extern scmObject getCdr(scmObject hopefullyCons);

extern scmObject SCM_NULL;
extern scmObject SCM_TRUE;
extern scmObject SCM_FALSE;
extern scmObject SCM_INV;
extern scmObject TOP_ENV;

extern jmp_buf savebuf;

extern scmObject *evalStack;
extern int evalStackCapacity;
extern int evalStackPointer;
extern int rememberEvalStackPointer;

extern void initializeEvalStack();
extern void pushToEvalStack();
extern scmObject popFromEvalStack();
extern void evalListAndPushToEvalStack(scmObject restList);
extern void pushListToEvalStack(scmObject restList);

extern scmObject PLUS(int nArgs);
extern scmObject MINUS(int nArgs);
extern scmObject MULT(int nArgs);
extern scmObject CONS(int nArgs);
extern scmObject CAR(int nArgs);
extern scmObject CDR(int nArgs);
extern scmObject EVAL(int nArgs);