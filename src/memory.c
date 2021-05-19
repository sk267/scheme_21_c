#include "scheme.h"

// #define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_CODE(code) code
#else
#define MEMORY_DEBUG_CODE(code)
#endif

int INITIAL_NUMBER_OF_MAX_SYMBOLS = 20;
scmObject *existingSymbols;
int numberOfExisitingSymbols = 0;
int currentNumberOfMaxSymbols;

scmObject newSymbolAllocation(char *input, int length)
{

    MEMORY_DEBUG_CODE({
        printf("----------------------------------------newSymbolAllocation betreten\n");
    })

    char *copiedString = (char *)malloc((length + 1) * sizeof(char));
    strcpy(copiedString, input);
    copiedString[length] = '\0';

    MEMORY_DEBUG_CODE(
        {
            printf("newSymbol: length: %d\n", length);
            printf("newSymbol: copied string: %s\n", copiedString);
        })

    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_SYMBOL;
    o->value.scmSymbol = copiedString;

    existingSymbols[numberOfExisitingSymbols] = o;

    MEMORY_DEBUG_CODE(
        {
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
    // printf("size: %d\n", sizeof(scmObject) * INITIAL_NUMBER_OF_MAX_SYMBOLS);
    existingSymbols = (scmObject *)malloc(sizeof(scmObject) * INITIAL_NUMBER_OF_MAX_SYMBOLS);
    // printf("size: %p\n", existingSymbols);
    currentNumberOfMaxSymbols = INITIAL_NUMBER_OF_MAX_SYMBOLS;
}

void initializeFunctions()
{
    defineEnvironmentValue(newSymbol("+", 1), newFunc(F_TAG_PLUS), TOP_ENV);
    defineEnvironmentValue(newSymbol("-", 1), newFunc(F_TAG_MINUS), TOP_ENV);
    defineEnvironmentValue(newSymbol("*", 1), newFunc(F_TAG_MULT), TOP_ENV);
    defineEnvironmentValue(newSymbol("cons", 4), newFunc(F_TAG_CONS), TOP_ENV);
    defineEnvironmentValue(newSymbol("car", 3), newFunc(F_TAG_CAR), TOP_ENV);
    defineEnvironmentValue(newSymbol("cdr", 3), newFunc(F_TAG_CDR), TOP_ENV);
    defineEnvironmentValue(newSymbol("eval", 4), newFunc(F_TAG_EVAL), TOP_ENV);
}

void initializeSyntax()
{
    defineEnvironmentValue(newSymbol("define", 6), newSyntax(S_TAG_DEFINE), TOP_ENV);
    defineEnvironmentValue(newSymbol("quote", 5), newSyntax(S_TAG_QUOTE), TOP_ENV);
    defineEnvironmentValue(newSymbol("set!", 4), newSyntax(S_TAG_SET), TOP_ENV);
    defineEnvironmentValue(newSymbol("display", 7), newSyntax(S_TAG_DISPLAY), TOP_ENV);
    defineEnvironmentValue(newSymbol("if", 2), newSyntax(S_TAG_IF), TOP_ENV);
}

void growSymbolTableBuffer()
{
    // printf("growSymbolTableBuffer betreten\n");
    // printf("growSymbolTableBuffer: %d\n", currentNumberOfMaxSymbols);
    currentNumberOfMaxSymbols += 10;
    // printf("growSymbolTableBuffer: %d\n", currentNumberOfMaxSymbols);
    // printf("sizeof exisitingSymbols: %ld\n", sizeof(existingSymbols));

    existingSymbols = (scmObject *)realloc(existingSymbols, sizeof(scmObject) * currentNumberOfMaxSymbols);
    // printf("Noch da\n");
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
            MEMORY_DEBUG_CODE({
                printf("--------------------------------------Found an existing Symbol!\n");
            })
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

scmObject newFunc(int whichFunction)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_FUNC;
    o->value.scmFunction.whichFunction = whichFunction;

    return o;
}

scmObject newSyntax(int whichSyntax)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_SYN;
    o->value.scmSyntax.whichSyntax = whichSyntax;

    return o;
}