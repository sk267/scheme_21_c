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
