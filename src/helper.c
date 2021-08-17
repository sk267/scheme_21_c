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
    if ((long)something & PTAG_SMALLINTEGER)
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
    bool isSmall;

    if ((long)something & PTAG_SMALLINTEGER)
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
    return (SCM_INT)something >> 1;
}

scmObject generateSmallInteger(SCM_INT intVal)
{
    long newSmallInt;

    newSmallInt = (long)intVal << 1;
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
        scm_print(expr);
    }

    fclose(FILE_POINTER);
}

void switchToReadFromString()
{

    WHICH_READ_V->readFromFile = false;
    WHICH_READ_V->readFromConsole = false;
    WHICH_READ_V->readString = true;
}