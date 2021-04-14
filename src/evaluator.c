#include "scheme.h"

scmObject scm_eval(scmObject inputUnevaluated)
{
    // scmObject car;
    // scmObject restList;
    // scmObject nextArg;
    // int sum;

    // if (inputUnevaluated->tag == TAG_CONS)
    // {
    //     car = inputUnevaluated->value.scmCons.car;
    //     restList = inputUnevaluated->value.scmCons.cdr;

    //     // Wenn es eine Cons ist muss man was machen
    //     if (car == newSymbol("+", 1))
    //     {
    //         while (restList->tag != TAG_NULL)
    //         {
    //             sum += restList.
    //         }
    //     }
    // }

    // Der ganze Rest evaluiert einfach zu sich selbst (Int, True, ...)

    return inputUnevaluated;
}
