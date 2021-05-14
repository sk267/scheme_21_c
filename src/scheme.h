#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>

#define scmAssert(ok, msg) scmAssertVerbose(ok, msg, __FILE__, __LINE__)

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
    TAG_ENV
};

typedef struct scmObjectStruct *scmObject;
typedef struct consStruct cons;
typedef struct envStruct envStr;

// struct envStruct
// {
//     scmObject *cons;
//     int capacity;
//     int nSymbols;
// };

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
    } value;
};

extern scmObject *existingSymbols;

extern void selftest();
extern char nextChar();
extern void unreadChar(char input);

extern scmObject newInteger(int inInt);
extern scmObject newString(char *input, int length);
extern scmObject newSymbol(char *input, int length);
extern scmObject newSymbolAllocation(char *input, int length);
extern scmObject newCons(scmObject car, scmObject cdr);
extern scmObject allocateEnvironment(int inCapacitiy);
extern void initializeTopEnv();
extern void setEnvironmentValue(scmObject key, scmObject value, scmObject env);
extern scmObject getEnvironmentValue(scmObject key, scmObject env);
extern int hashForEnv(scmObject symbol, scmObject env);

extern void scmAssertVerbose(int ok, char *msg, char *file, int line);

extern void initializeSymbolTableBuffer();

extern scmObject scm_read();
extern scmObject scm_eval(scmObject input);
extern void scm_print(scmObject input);

extern scmObject SCM_NULL;
extern scmObject SCM_TRUE;
extern scmObject SCM_FALSE;
extern scmObject SCM_INV;
extern scmObject TOP_ENV;

extern jmp_buf savebuf;
