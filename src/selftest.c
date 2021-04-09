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

    scmObject obj1 = newInteger(33);

    if (obj1->tag != TAG_INT)
    {
        char *tmpString = "newInteger wrong Tag";
        scmError(tmpString);
    }
    if (obj1->value.scmInt != 33)
    {
        char tmpString[] = "newInteger wrong Value";
        scmError(tmpString);
    }

    obj1 = newCons(newInteger(44), newSymbol("hallo", 5));
    scm_print(obj1);

    printf("#################### selftest ends ###########################\n");
}