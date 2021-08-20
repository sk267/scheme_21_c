#include "scheme.h"

scmObject getCar(scmObject hopefullyCons)
{
    scmAssert(getTag(hopefullyCons) == TAG_CONS, "expected a Cons!");
    return hopefullyCons->value.scmCons.car;
}

scmObject getCdr(scmObject hopefullyCons)
{
    scmAssert(getTag(hopefullyCons) == TAG_CONS, "expected a Cons!");
    return hopefullyCons->value.scmCons.cdr;
}

int getTag(scmObject something)
{
    uintptr_t tmpPointerToDoAddOperation = (uintptr_t)something;
    if (tmpPointerToDoAddOperation & PTAG_SMALLINTEGER)
    {
        // Unterstes Bit ist gesetzt
        // es handelt sich um ein SMALL_INTEGER
        return TAG_SMALLINT;
    }
    // Ansonsten liefere das normale Tag zurück
    return something->tag;
}

bool isSmallInteger(scmObject something)
{
    uintptr_t tmpPointerToDoAddOperation = (uintptr_t)something;
    if (tmpPointerToDoAddOperation & PTAG_SMALLINTEGER)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isInteger(scmObject maybeInteger)
{
    if (isSmallInteger(maybeInteger))
    {
        return true;
    }
    else if (maybeInteger->tag == TAG_INT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool fitsForForSmallInt(int number)
{
    if ((number <= MAX_SMALL_INT) && (number >= MIN_SMALL_INT))
    {
        return true;
    }
    else
    {
        return false;
    }
}

SCM_INT getSmallIntegerValue(scmObject something)
{
    scmAssert((int)isSmallInteger(something), "did not get a small integer!");
    // bits müssen wieder um eins nach rechts geshiftet werden, um korrekte Zahl zu erhalten
    uintptr_t tmpVarForBitShifting = (uintptr_t)something;
    tmpVarForBitShifting = tmpVarForBitShifting >> 1;
    return (SCM_INT)tmpVarForBitShifting;
}

scmObject generateSmallInteger(SCM_INT intVal)
{
    uintptr_t newSmallInt;

    newSmallInt = (uintptr_t)intVal << 1;
    newSmallInt = newSmallInt | PTAG_SMALLINTEGER;

    return (scmObject)newSmallInt;
}

int getIntVal(scmObject smallOrBigInt)
{
    if (isSmallInteger(smallOrBigInt))
    {
        return getSmallIntegerValue(smallOrBigInt);
    }
    else
    {
        scmAssert(getTag(smallOrBigInt) == TAG_INT, "Did not get an int Value");
        return smallOrBigInt->value.scmInt;
    }
}

void switchToReadFromConsole()
{
    WHICH_READ_V->readFromFile = false;
    WHICH_READ_V->readFromConsole = true;
    WHICH_READ_V->readString = false;
}

void scmReadFile(char *fileName)
{

    FILE_POINTER = fopen(fileName, "r");

    WHICH_READ_V->readFromFile = true;
    WHICH_READ_V->readFromConsole = false;
    WHICH_READ_V->readString = false;

    while (SINGLE != EOF)
    {
        // printf("scmReadFile: SINGLE: %c\n", SINGLE);
        scmObject expr;

        expr = scm_read();
        expr = scm_eval(expr, TOP_ENV);
    }

    fclose(FILE_POINTER);
}

void switchToReadFromString()
{

    WHICH_READ_V->readFromFile = false;
    WHICH_READ_V->readFromConsole = false;
    WHICH_READ_V->readString = true;
}

void print_env(scmObject env)
{
    scmObject binding, key, val;
    while (env != SCM_NULL)
    {
        printf("------------------%p\n", env);
        for (int i = 0; i < env->value.scmEnv.capacity; i++)
        {
            if (env->value.scmEnv.keyValuePairs[i] == NULL)
            {
                continue;
            }
            binding = env->value.scmEnv.keyValuePairs[i];
            key = getCar(binding);
            val = getCdr(binding);
            printf("key: ");
            scm_print(key);
            printf(", value: ");
            scm_print(val);
            printf("\n");
        }
        printf("parent: %p\n", env->value.scmEnv.parentEnv);
        env = env->value.scmEnv.parentEnv;
    }
}