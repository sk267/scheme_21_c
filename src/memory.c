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
            printf("newSymbolAllocation: numberOfExisitingSymbols: %d\n", numberOfExisitingSymbols);
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
    defineEnvironmentValue(newSymbol("+", 1), newFunc(PLUS, "+"), TOP_ENV);
    defineEnvironmentValue(newSymbol("=", 1), newFunc(EQNR, "="), TOP_ENV);
    defineEnvironmentValue(newSymbol("-", 1), newFunc(MINUS, "-"), TOP_ENV);
    defineEnvironmentValue(newSymbol(">", 1), newFunc(GTNR, ">"), TOP_ENV);
    defineEnvironmentValue(newSymbol("<", 1), newFunc(SMNR, "<"), TOP_ENV);
    defineEnvironmentValue(newSymbol("*", 1), newFunc(MULT, "*"), TOP_ENV);
    defineEnvironmentValue(newSymbol("cons", 4), newFunc(CONS, "cons"), TOP_ENV);
    defineEnvironmentValue(newSymbol("car", 3), newFunc(CAR, "car"), TOP_ENV);
    defineEnvironmentValue(newSymbol("cdr", 3), newFunc(CDR, "cdr"), TOP_ENV);
    defineEnvironmentValue(newSymbol("eval", 4), newFunc(EVAL, "eval"), TOP_ENV);
    defineEnvironmentValue(newSymbol("eq?", 3), newFunc(EQ, "eq?"), TOP_ENV);
}

void initializeSyntax()
{
    defineEnvironmentValue(newSymbol("define", 6), newSyntax(DEFINE, "define"), TOP_ENV);
    defineEnvironmentValue(newSymbol("quote", 5), newSyntax(QUOTE, "quote"), TOP_ENV);
    defineEnvironmentValue(newSymbol("set!", 4), newSyntax(SET, "set"), TOP_ENV);
    defineEnvironmentValue(newSymbol("display", 7), newSyntax(DISPLAY, "display"), TOP_ENV);
    defineEnvironmentValue(newSymbol("if", 2), newSyntax(IF, "if"), TOP_ENV);
    defineEnvironmentValue(newSymbol("lambda", 6), newSyntax(LAMBDA, "lambda"), TOP_ENV);
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

static scmObject generateBigInteger(int inInt)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_INT;
    o->value.scmInt = inInt;

    return o;
}

scmObject newInteger(int inInt)
{
    if (fitsForForSmallInt(inInt))
    {
        return generateSmallInteger(inInt);
    }
    else
    {
        return generateBigInteger(inInt);
    }
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

scmObject newFunc(scmObjectFunctionPointer codeToUse, char *funcName)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_FUNC;
    o->value.scmFunction.functionName = funcName;
    o->value.scmFunction.code = codeToUse;

    return o;
}

scmObject newSyntax(scmObjecttSyntaxPointer codeToUse, char *synName)
{
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_SYN;
    o->value.scmSyntax.syntaxName = synName;
    o->value.scmSyntax.code = codeToUse;

    return o;
}

scmObject newUserDefinedFunction(scmObject argList, scmObject bodyList, scmObject homeEnv)
{

    // printf("betrete newUserDefinedFunction \n");
    scmObject o = (scmObject)malloc(sizeof(struct scmObjectStruct));
    o->tag = TAG_USERDEFINDEFUNC;
    o->value.scmUserDefindedFunction.argList = argList;
    o->value.scmUserDefindedFunction.bodyList = bodyList;
    o->value.scmUserDefindedFunction.homeEnv = homeEnv;

    return o;
}