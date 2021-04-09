
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum
{
    TAG_INT = 1,
    TAG_DOUBLE,
    TAG_STRING,
    TAG_SYMBOL,
    TAG_CONS,
    TAG_TRUE,
    TAG_FALSE,
    TAG_NULL
};

typedef struct scmObjectStruct *scmObject;

struct scmObjectStruct
{
    int tag;

    union scmObjectUnion
    {
        int scmInt;
        char *scmChar;
        char *scmSymbol;
        double scmDouble;
    } value;
};

extern void selftest();
extern char nextChar();
extern void unreadChar(char input);

extern scmObject newInteger(int inInt);
extern scmObject newString(char *input);
extern scmObject newSymbol(char *input, int length);

extern scmObject scm_read();
extern scmObject scm_eval(scmObject input);
extern void scm_print(scmObject input);

extern scmObject SCM_NULL;
extern scmObject SCM_TRUE;
extern scmObject SCM_FALSE;