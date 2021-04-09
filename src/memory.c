#include "scheme.h"

scmObject newSymbol(char *input, int length)
{
    // wieso geht hier malloc(1)??? Da dürfte das doch garnicht reingehen!?
    char *copiedString = (char *)malloc(length);
    strcpy(copiedString, input);

    // printf("newSymbol: input: %s\n", input);

    scmObject o = (scmObject)malloc(sizeof(scmObject));
    o->tag = TAG_SYMBOL;
    o->value.scmSymbol = copiedString;

    return o;
}

scmObject newInteger(int inInt)
{

    scmObject o = (scmObject)malloc(sizeof(scmObject));
    o->tag = TAG_INT;
    o->value.scmInt = inInt;

    return o;
}

scmObject newString(char *input)
{

    char *copiedString = (char *)malloc(16);
    strcpy(copiedString, input);

    scmObject o = (scmObject)malloc(sizeof(scmObject));
    o->tag = TAG_STRING;
    o->value.scmSymbol = copiedString;

    return o;
}
