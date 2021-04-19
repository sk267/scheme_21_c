#include "scheme.h"

// #define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_CODE(code) code
#else
#define MEMORY_DEBUG_CODE(code)
#endif

int INITIAL_NUMBER_OF_MAX_SYMBOLS = 50;
scmObject *existingSymbols;
int numberOfExisitingSymbols = 0;
int currentNumberOfMaxSymbols;

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

    MEMORY_DEBUG_CODE({
        printf("newSymbolAllocation: gerade eben neues Symbol hinzugefuegt:\n");
        printf("%d\n", numberOfExisitingSymbols);
        scm_print(existingSymbols[numberOfExisitingSymbols]);
        printf("\n");
    })

    numberOfExisitingSymbols++;

    return o;
}
void initializeSymbolTableBuffer()
{
    existingSymbols = (scmObject *)malloc(sizeof(scmObject *) * INITIAL_NUMBER_OF_MAX_SYMBOLS);
    currentNumberOfMaxSymbols = INITIAL_NUMBER_OF_MAX_SYMBOLS;
}

void growSymbolTableBuffer()
{
    printf("growSymbolTableBuffer betreten\n");
    printf("growSymbolTableBuffer: %d\n", currentNumberOfMaxSymbols);
    currentNumberOfMaxSymbols += 1;
    printf("growSymbolTableBuffer: %d\n", currentNumberOfMaxSymbols);
    // printf("sizeof exisitingSymbols: %ld\n", sizeof(existingSymbols));

    existingSymbols = (scmObject *)realloc(existingSymbols, currentNumberOfMaxSymbols);
    printf("Noch da\n");
}

scmObject newSymbol(char *input, int length)
{

    if (numberOfExisitingSymbols == currentNumberOfMaxSymbols)
    {
        // SymbolTableBuffer ist voll, der Buffer muss vergrößert werden
        growSymbolTableBuffer();
    }

    // Check, if this Symbol already exists in SymbolTable
    for (int i = 0; i < numberOfExisitingSymbols; i++)
    {

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
