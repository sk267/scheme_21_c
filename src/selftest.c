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
    printf("#################### selftest begins ###########################\n\n");
    // printf("IHK.value: %s\n", newSymbol("IHK", 3)->value.scmSymbol);
    // printf("\nlllllllllllllllllllllllllllllll\n\n");

    // char *a = "Hallo";
    // char *b = "Hallo";
    // printf("selftest: strcmp: %d\n", strcmp(a, b));

    printf("newSymbol(111, 3); wird aufgerufen\n");
    newSymbol("111", 3);

    printf("newSymbol(222, 3); wird aufgerufen\n");
    newSymbol("222", 3);
    printf("newSymbol(111, 3); wird aufgerufen\n");
    newSymbol("111", 3);

    printf("newSymbol(333, 3); wird aufgerufen\n");
    newSymbol("333", 3);
    printf("newSymbol(444, 3); wird aufgerufen\n");
    newSymbol("444", 3);
    printf("newSymbol(555, 3); wird aufgerufen\n");
    newSymbol("555", 3);

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
    printf("\n");

    obj1 = newCons(newInteger(33), newCons(newInteger(11), newSymbol("hallo2", 6)));
    scm_print(obj1);
    printf("\n");

    printf("Strcmp check: %d\n", strcmp("Hallo", "Hallo"));
    printf("Strcmp check: %d\n", strcmp("Hallo", "Hello"));

    printf("#################### selftest ends ###########################\n");
}