#include "scheme.h"

// #define PRINTER_DEBUG

#ifdef PRINTER_DEBUG
#define PRINTER_DEBUG_CODE(code) code
#else
#define PRINTER_DEBUG_CODE(code) /*as nothing*/
#endif

void scm_print(scmObject input)
{
    switch (input->tag)
    {

    case TAG_INV:
        break;
    case TAG_INT:
        PRINTER_DEBUG_CODE({
            printf("scm_print: integer erkannt\n");
        })
        printf("%d", input->value.scmInt);
        break;
    case TAG_STRING:
        PRINTER_DEBUG_CODE({
            printf("scm_print: String erkannt\n");
        })
        printf("\"");
        printf("%s", input->value.scmChar);
        printf("\"");
        break;
    case TAG_SYMBOL:
        printf("%s", input->value.scmSymbol);
        break;
    case TAG_NULL:
        printf("SCM_NULL");
        break;
    case TAG_FALSE:
        printf("#f");
        break;
    case TAG_TRUE:
        printf("#t");
        break;
    case TAG_CONS:
        printf("(");
        scm_print(input->value.scmCons.car);
        printf(" ");
        scm_print(input->value.scmCons.cdr);
        printf(")");
        break;

    case TAG_FUNC:
        printf("print: FUNC gefunden!\n");
        printf("Function Tag: %d\n", input->value.scmFunction.whichFunction);
        break;

    case TAG_SYN:
        printf("print: SYN gefunden!\n");
        printf("Syntax Tag: %d\n", input->value.scmSyntax.whichSyntax);
        break;

    default:
        printf("kann ich noch nicht printen!! \n");
        break;
    }
}