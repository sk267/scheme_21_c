#include "scheme.h"

// #define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_CODE(code) code
#else
#define MEMORY_DEBUG_CODE(code)
#endif

scmObject newSymbol(char *input, int length)
{

    char *copiedString = (char *)malloc((length + 1) * sizeof(char));
    strcpy(copiedString, input);
    copiedString[length] = '\0';

    MEMORY_DEBUG_CODE({
        printf("newSymbol: length: %d\n", length);
        printf("newSymbol: copied string: %s\n", copiedString);
    })

    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_SYMBOL;
    o->value.scmSymbol = copiedString;

    return o;
}

scmObject newInteger(int inInt)
{

    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_INT;
    o->value.scmInt = inInt;

    return o;
}

scmObject newString(char *input, int length)
{

    char *copiedString = (char *)malloc((length + 1) * sizeof(char));
    strcpy(copiedString, input);
    copiedString[length] = '\0';

    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_STRING;
    o->value.scmSymbol = copiedString;

    return o;
}

scmObject newCons(scmObject inCar, scmObject inCdr)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_CONS;
    o->value.scmCons.car = inCar;
    o->value.scmCons.cdr = inCdr;
    return o;
}
