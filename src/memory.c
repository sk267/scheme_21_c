#include "scheme.h"

// #define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_CODE(code) code
#else
#define MEMORY_DEBUG_CODE(code)
#endif

int INITAL_SYMBOL_TABLE_CAPACITY = 30;
scmObject *symbolTable;
scmObject *biggerMem;
int numberOfSymbolsInSymbolTable = 0;
int symbolTableCapacity;

static inline void printSymbolTable()
{
    printf("SymbolTable sieht so aus: \n");
    for (int i = 0; i < symbolTableCapacity; i++)
    {
        printf("i: %d ", i);
        if (symbolTable[i] == NULL)
        {
            printf("NULL");
        }
        else
        {
            scm_print(symbolTable[i]);
        }
        printf("\n");
    }
}

static inline void printBiggerMem()
{
    printf("printBiggerMem sieht so aus: \n");
    for (int i = 0; i < symbolTableCapacity; i++)
    {
        printf("i: %d ", i);
        if (biggerMem[i] == NULL)
        {
            printf("NULL");
        }
        else
        {
            scm_print(biggerMem[i]);
        }
        printf("\n");
    }
}

scmObject newSymbolAllocation(char *input, int length, int indexToStart)
{

    MEMORY_DEBUG_CODE({})
    // printf("----------------------------------------newSymbolAllocation betreten\n");

    // String zusammenbauen und mit '\0' byte ans Ende setzen
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

    // Geeignete Stelle im Symbol-Table suchen
    int idx = indexToStart;
    while (true)
    {
        // ist an diesem Index eine Stelle frei?
        if (symbolTable[idx % symbolTableCapacity] == NULL)
        {
            // lege das Symbol an dieser Stelle ab
            symbolTable[idx % symbolTableCapacity] = o;
            numberOfSymbolsInSymbolTable++;
            break;
        }
        else
        {
            // suche weiter
            idx++;
        }
        if ((idx % symbolTableCapacity) == indexToStart)
        {
            // should never happen
            scmError("No free slot found for new Symbol in Symbol Table!");
        }
    }

    MEMORY_DEBUG_CODE(
        {
            printf("newSymbolAllocation: gerade eben neues Symbol hinzugefuegt:\n");
            printf("newSymbolAllocation: numberOfSymbolsInSymbolTable: %d\n", numberOfSymbolsInSymbolTable);
            scm_print(symbolTable[numberOfSymbolsInSymbolTable]);
            printf("\n");
        })

    return o;
}
void initializeSymbolTableBuffer()
{
    symbolTable = (scmObject *)calloc(INITAL_SYMBOL_TABLE_CAPACITY, sizeof(scmObject));
    symbolTableCapacity = INITAL_SYMBOL_TABLE_CAPACITY;
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
    scmObject *oldMemory = symbolTable;
    MEMORY_DEBUG_CODE(
        {
            printf("-------------------------------------growSymbolTableBuffer betreten\n");
            printf("growSymbolTableBuffer: %d\n", symbolTableCapacity);
            printSymbolTable();
        })
    int oldSymbolTableCapacity = symbolTableCapacity;
    symbolTableCapacity += 10;

    biggerMem = (scmObject *)calloc(symbolTableCapacity, sizeof(scmObject));
    memcpy(biggerMem, symbolTable, sizeof(scmObject) * oldSymbolTableCapacity);
    symbolTable = biggerMem;

    free(oldMemory);
}

static int hashForSymbols(char *input, int length)
{
    int myHash;
    myHash = (uintptr_t)input * 7;
    myHash = myHash % symbolTableCapacity;
    return myHash;
}

scmObject newSymbol(char *input, int length)
{

    // SymbolTable sollte maximal zu 70% voll sein!
    if (numberOfSymbolsInSymbolTable >= (symbolTableCapacity * 0.7))
    {
        growSymbolTableBuffer();
    }

    int indexToStart = hashForSymbols(input, length) % symbolTableCapacity;

    // Check, if this Symbol already exists in SymbolTable
    for (int i = 0; i < symbolTableCapacity; i++)
    {
        if (symbolTable[(indexToStart + i) % symbolTableCapacity] != NULL)
        {
            if (getTag(symbolTable[(indexToStart + i) % symbolTableCapacity]) == TAG_SYMBOL)
            {

                // printf("newSymbol: existens check: Symbol at idx %d: ", i);
                // scm_print(symbolTable[(indexToStart + i) % symbolTableCapacity]);
                // printf("\n");

                if (strcmp(symbolTable[(indexToStart + i) % symbolTableCapacity]->value.scmSymbol, input) == 0)
                {
                    // Found an existing Symbol!
                    MEMORY_DEBUG_CODE({
                        printf("--------------------------------------Found an existing Symbol!\n");
                    })
                    return symbolTable[(indexToStart + i) % symbolTableCapacity];
                }
            }
        }
        else
        {
            continue;
        }
    }
    // No exisitng Symbol found, new Symbol will be created!
    return newSymbolAllocation(input, length, indexToStart);
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