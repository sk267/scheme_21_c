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

    // printf("selftest: BITSHIFT: %d\n", 2 >> 1);
    // printf("selftest: BITSHIFT: %d\n", 2 << 1);
    // printf("selftest: BITSHIFT: %d\n", 2 << 2);

    scmObject symbol1, symbol2, symbol3, symbol4, symbol5;

    // printf("newSymbol(111, 3); wird aufgerufen\n");
    symbol1 = newSymbol("111", 3);

    // printf("newSymbol(222, 3); wird aufgerufen\n");
    symbol2 = newSymbol("222", 3);

    // printf("newSymbol(111, 3); wird aufgerufen\n");
    symbol3 = newSymbol("111", 3);

    // printf("%p\n", symbol1);
    // printf("%p\n", symbol3);
    scmAssert(symbol1 == symbol3, "Pointer sollten die gleiche Adresse haben!!\n");
    scmAssert(symbol1 != symbol2, "Pointer sollten NICHT die gleiche Adresse haben!!\n");

    // printf("newSymbol(333, 3); wird aufgerufen\n");
    symbol4 = newSymbol("333", 3);
    scmAssert(symbol1 != symbol4, "Pointer sollten NICHT die gleiche Adresse haben!!\n");

    // printf("newSymbol(444, 3); wird aufgerufen\n");
    symbol5 = newSymbol("444", 3);
    scmAssert(symbol1 != symbol5, "Pointer sollten NICHT die gleiche Adresse haben!!\n");

    scmObject obj1 = newInteger(33);

    scmAssert(obj1->tag == TAG_INT, "newInteger should be TAG_INT");

    scmAssert(obj1->value.scmInt == 33, "newInteger value should be 33");

    obj1 = newCons(newInteger(44), newSymbol("hallo", 5));
    // scm_print(obj1);
    // printf("\n");

    obj1 = newCons(newInteger(33), newCons(newInteger(11), newSymbol("hallo2", 6)));
    // scm_print(obj1);
    // printf("\n");

    scmObject tmpKey1 = newSymbol("var1", 4);
    scmObject tmpVal1 = newInteger(11);
    setEnvironmentValue(tmpKey1, tmpVal1, TOP_ENV);

    scmObject tmpKey2 = newSymbol("var2", 4);
    scmObject tmpVal2 = newInteger(22);
    setEnvironmentValue(tmpKey2, tmpVal2, TOP_ENV);

    tmpVal1 = getEnvironmentValue(tmpKey1, TOP_ENV);
    scm_print(tmpVal1);

    tmpVal2 = getEnvironmentValue(tmpKey2, TOP_ENV);
    scm_print(tmpVal2);

    scmObject symbolToHash1;
    scmObject symbolToHash2;
    scmObject symbolToHash3;

    symbolToHash1 = newSymbol("abc", 3);
    symbolToHash2 = newSymbol("abc", 3);
    symbolToHash3 = newSymbol("acb", 3);

    // myHash = (long)symbolToHash1->value.scmSymbol;
    printf("\nMyHash1: %p\n", symbolToHash1);
    // myHash = (long)symbolToHash2->value.scmSymbol;
    printf("\nMyHash2: %p\n", symbolToHash2);

    hashForEnv(symbolToHash1, TOP_ENV);
    hashForEnv(symbolToHash2, TOP_ENV);
    hashForEnv(symbolToHash3, TOP_ENV);

    printf("#################### selftest ends ###########################\n");
}