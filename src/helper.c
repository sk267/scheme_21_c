#include "scheme.h"

scmObject getCar(scmObject hopefullyCons)
{
    scmAssert(hopefullyCons->tag == TAG_CONS, "expected a Cons!");
    return hopefullyCons->value.scmCons.car;
}

scmObject getCdr(scmObject hopefullyCons)
{
    scmAssert(hopefullyCons->tag == TAG_CONS, "expected a Cons!");
    return hopefullyCons->value.scmCons.cdr;
}

int getTag(scmObject something)
{
    if (something->tag & PTAG_SMALLINTEGER)
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
    return (long)something & PTAG_SMALLINTEGER;
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
        scmAssert(smallOrBigInt->tag == TAG_INT, "Did not get an int Value");
        return smallOrBigInt->value.scmInt;
    }
}