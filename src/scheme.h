#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>

#define scmAssert(ok, msg) scmAssertVerbose(ok, msg, __FILE__, __LINE__)
#define scmError(msg) scmErrorVerbose(msg, __FILE__, __LINE__)

#define PTAG_SMALLINTEGER 1
#define PTAG_OBJECT 0
#define SCM_INT int // könnte auch ein long sein, aber nur wenn die Rechner-Architektur daszulässt (hier erstmal nur int gemacht)

#define MAX_SMALL_INT 0x3FFFFFFFL
#define MIN_SMALL_INT (-MAX_SMALL_INT - 1)

extern bool readFromFile;

enum
{
    TAG_INT = 1,
    TAG_SMALLINT,
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
    TAG_SYN,
    TAG_USERDEFINDEFUNC
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

struct whichRead
{
    bool readFromConsole;
    bool readFromFile;
    bool readString;
};

typedef struct scmObjectStruct *scmObject;
typedef struct consStruct cons;
typedef struct envStruct envStr;
typedef struct funcStruct scmFunc;
typedef struct synStruct scmSyn;
typedef struct userDefinedFuncStruct scmUserDefinedFunc;
typedef scmObject (*scmObjectFunctionPointer)();
typedef scmObject (*scmObjecttSyntaxPointer)();

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
    scmObject parentEnv;
};

struct funcStruct
{
    char *functionName;
    scmObjectFunctionPointer code;
};

struct synStruct
{
    char *syntaxName;
    scmObjecttSyntaxPointer code;
};

struct userDefinedFuncStruct
{
    scmObject argList;
    scmObject bodyList;
    scmObject homeEnv;
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
        scmUserDefinedFunc scmUserDefindedFunction;
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
extern scmObject newFunc(scmObjectFunctionPointer codeToUse, char *functionName);
extern scmObject newSyntax(scmObjectFunctionPointer codeToUse, char *syntaxName);
extern scmObject newUserDefinedFunction(scmObject argList, scmObject bodyList, scmObject homeEnv);

extern scmObject allocateEnvironment(int inCapacitiy, scmObject parentEnv);
extern void initializeTopEnv();

extern void initializeFunctions();
extern void initializeSyntax();

extern void defineEnvironmentValue(scmObject key, scmObject value, scmObject env);
extern void setEnvironmentValue(scmObject key, scmObject value, scmObject env);
extern scmObject getEnvironmentValue(scmObject key, scmObject env);
extern int hashForEnv(scmObject symbol, scmObject env);
extern scmObject evalFuncOrSyntax(scmObject exprUnevaluated, scmObject env);
extern scmObject evalFunction(scmObject functionEvaluated, scmObject restList, scmObject env);
extern scmObject evalSyntax(scmObject functionEvaluated, scmObject restList, scmObject env);
extern scmObject evalUserDefinedFunction(scmObject funcOrSynEvaluated, scmObject restList, scmObject env);

extern void scmAssertVerbose(int ok, char *msg, char *file, int line);
extern void scmErrorVerbose(char *msg, char *file, int line);

extern void initializeSymbolTableBuffer();

extern scmObject scm_read();
extern scmObject scm_eval(scmObject input, scmObject env);
extern void scm_print(scmObject input);

extern scmObject getCar(scmObject hopefullyCons);
extern scmObject getCdr(scmObject hopefullyCons);
extern scmObject generateSmallInteger(SCM_INT intVal);
extern SCM_INT getSmallIntegerValue(scmObject something);
extern bool isSmallInteger(scmObject something);
extern int getTag(scmObject something);
extern bool fitsForForSmallInt(int number);
extern int getIntVal(scmObject smallOrBigInt);
extern bool isInteger(scmObject maybeInteger);

extern scmObject SCM_NULL;
extern scmObject SCM_TRUE;
extern scmObject SCM_FALSE;
extern scmObject SCM_INV;
extern scmObject TOP_ENV;
extern struct whichRead *WHICH_READ_V;
extern FILE *FILE_POINTER;
extern int SINGLE;

extern jmp_buf savebuf;

extern scmObject *evalStack;
extern int evalStackCapacity;
extern int evalStackPointer;
extern int rememberEvalStackPointer;

extern void initializeEvalStack();
extern void pushToEvalStack();
extern scmObject popFromEvalStack();
extern void evalListAndPushToEvalStack(scmObject restList, scmObject env);
extern void pushListToEvalStack(scmObject restList, scmObject env);

extern void scmReadFile(char *fileName);
extern void switchToReadFromConsole();
extern void switchToReadFromString();

extern scmObject EQ(int nArgs);
extern scmObject EQNR(int nArgs);
extern scmObject GTNR(int nArgs);
extern scmObject SMNR(int nArgs);
extern scmObject PLUS(int nArgs);
extern scmObject MINUS(int nArgs);
extern scmObject MULT(int nArgs);
extern scmObject CONS(int nArgs);
extern scmObject CAR(int nArgs);
extern scmObject CDR(int nArgs);
extern scmObject EVAL(int nArgs);

extern scmObject DEFINE(int nArgs, scmObject env);
extern scmObject SET(int nArgs, scmObject env);
extern scmObject DISPLAY(int nArgs, scmObject env);
extern scmObject QUOTE(int nArgs, scmObject env);
extern scmObject IF(int nArgs, scmObject env);
extern scmObject LAMBDA(int nArgs, scmObject env);
