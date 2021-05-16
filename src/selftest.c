#include "scheme.h"

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

    // int *testInts = (int *)calloc(5, sizeof(int));

    // testInts[2] = 27;

    // for (int i = 0; i < 5; i++)
    // {

    //     printf("i: %d\n", i);
    //     if (testInts[i] == NULL)
    //     {
    //         printf("hier steht NULL drin\n");
    //     }
    //     else
    //     {
    //         printf("hier steht %d\n", testInts[i]);
    //     }
    // }

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
    scmObject tmpVal1 = newInteger(1);
    setEnvironmentValue(tmpKey1, tmpVal1, TOP_ENV);

    scmObject tmpKey2 = newSymbol("var2", 4);
    scmObject tmpVal2 = newInteger(2);
    setEnvironmentValue(tmpKey2, tmpVal2, TOP_ENV);

    scmObject tmpKey3 = newSymbol("var3", 4);
    scmObject tmpVal3 = newInteger(3);
    setEnvironmentValue(tmpKey3, tmpVal3, TOP_ENV);

    scmObject tmpKey4 = newSymbol("var4", 4);
    scmObject tmpVal4 = newInteger(4);
    setEnvironmentValue(tmpKey4, tmpVal4, TOP_ENV);

    scmObject tmpKey5 = newSymbol("var5", 4);
    scmObject tmpVal5 = newInteger(5);
    setEnvironmentValue(tmpKey5, tmpVal5, TOP_ENV);

    scmObject tmpKey6 = newSymbol("var6", 4);
    scmObject tmpVal6 = newInteger(6);
    setEnvironmentValue(tmpKey6, tmpVal6, TOP_ENV);

    tmpVal1 = getEnvironmentValue(tmpKey1, TOP_ENV);
    printf("\n");
    scm_print(tmpVal1);
    printf("\n");

    tmpVal2 = getEnvironmentValue(tmpKey2, TOP_ENV);
    printf("\n");
    scm_print(tmpVal2);
    printf("\n");

    tmpVal3 = getEnvironmentValue(tmpKey3, TOP_ENV);
    printf("\n");
    scm_print(tmpVal3);
    printf("\n");

    tmpVal4 = getEnvironmentValue(tmpKey4, TOP_ENV);
    printf("\n");
    scm_print(tmpVal4);
    printf("\n");

    tmpVal5 = getEnvironmentValue(tmpKey5, TOP_ENV);
    printf("\n");
    scm_print(tmpVal5);
    printf("\n");

    tmpVal6 = getEnvironmentValue(tmpKey6, TOP_ENV);
    printf("\n");
    scm_print(tmpVal6);
    printf("\n");

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