#include "scheme.h"

// #define PRINTER_DEBUG

#ifdef PRINTER_DEBUG
#define PRINTER_DEBUG_CODE(code) code
#else
#define PRINTER_DEBUG_CODE(code) /*as nothing*/
#endif

void scm_print(scmObject input)
{
    if (input->tag == TAG_INT)
    {
        // INTEGER ############################################
        PRINTER_DEBUG_CODE({
            printf("scm_print: integer erkannt\n");
        })
        printf("%d\n", input->value.scmInt);
    }
    else if (input->tag == TAG_STRING)
    {
        // STRING ############################################
        PRINTER_DEBUG_CODE({
            printf("scm_print: String erkannt\n");
        })
        printf("\"");
        printf("%s", input->value.scmChar);
        printf("\"\n");
    }
    else if (input->tag == TAG_SYMBOL)
    {
        // SYMBOL #############################################
        printf("%s\n", input->value.scmSymbol);
    }
    else if (input->tag == TAG_NULL)
    {
        printf("()\n");
    }
    else if (input->tag == TAG_FALSE)
    {
        printf("#f\n");
    }
    else if (input->tag == TAG_TRUE)
    {
        printf("#t\n");
    }
    else
    {
        printf("kann ich noch nicht printen!! \n");
    }
}