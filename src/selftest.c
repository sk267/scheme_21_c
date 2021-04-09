#include "scheme.h"

void scmError(char *message)
{
    printf("\n\n!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("%s\n", message);
    printf("!!!!!!!!!!!!!!!!!!!!!!\n\n");
}

void printTmp(char *input)
{
    printf("printTmp: %s\n", input);
}

void selftest()
{
    printf("#################### selftest begins ###########################\n");

    char *charBuffer = (char *)malloc(sizeof(char));
    charBuffer = "1234567891011121314151617181920";
    printTmp(charBuffer);

    scmObject obj1 = newInteger(33);

    if (obj1->tag != TAG_CONS)
    {
        char tmpString[] = "newInteger wrong Tag";
        scmError(tmpString);
    }
    if (obj1->value.scmInt != 33)
    {
        char tmpString[] = "newInteger wrong Value";
        scmError(tmpString);
    }

    char tmpString[] = "12345";
    scmObject obj2 = newSymbol(tmpString);
    if (obj2->tag != TAG_SYMBOL)
    {
        char tmpString[] = "newSymbol wrong Tag";
        scmError(tmpString);
    }
    if (obj2->value.scmSymbol != tmpString)
    {
        char tmpString[] = "newSymbol wrong value";
        scmError(tmpString);
    }

    printf("#################### selftest ends ###########################\n");
}