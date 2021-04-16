#include "scheme.h"

// #define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_CODE(code) code
#else
#define MEMORY_DEBUG_CODE(code)
#endif

scmObject *existingSymbols;
int numberOfExisitingSymbols = 0;

scmObject newSymbolAllocation(char *input, int length)
{

    printf("----------------------------------------newSymbolAllocation betreten\n");

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

    existingSymbols[numberOfExisitingSymbols] = o;

    printf("newSymbolAllocation: gerade eben neues Symbol hinzugefuegt:\n");

    printf("%d\n", numberOfExisitingSymbols);
    scm_print(existingSymbols[numberOfExisitingSymbols]);
    printf("\n");

    numberOfExisitingSymbols++;

    return o;
}

scmObject
newSymbol(char *input, int length)
{
    existingSymbols = (scmObject *)malloc(sizeof(scmObject *) * 4);
    // Check, if this Symbol already exists in SymbolTable
    for (int i = 0; i < numberOfExisitingSymbols; i++)
    {
        // printf("noch da\n");
        // printf("existingSymbols[%d]: ", i);
        // printf(".value: %s\n", existingSymbols[i]->value.scmChar);

        printf("strmp: %d\n", strcmp(existingSymbols[0]->value.scmSymbol, input));
        if (strcmp(existingSymbols[i]->value.scmSymbol, input) == 0)
        {
            // Found an existing Symbol!
            printf("--------------------------------------Found an existing Symbol!\n");
            return existingSymbols[i];
        }
    }
    // No exisitng Symbol found, new Symbol will be created!
    return (newSymbolAllocation(input, length));
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
