#include "scheme.h"

#ifdef MAIN_DEBUG
#define MAIN_DEBUG_CODE(code) code
#else
#define MAIN_DEBUG_CODE(code) // as nothing
#endif

scmObject SCM_TRUE;
scmObject SCM_FALSE;
scmObject SCM_NULL;
scmObject SCM_INV;

// Globale Variable; muss von überall erreichbar sein:
jmp_buf savebuf;

static void
scm_initialize()
{

    MAIN_DEBUG_CODE({
        printf("initialize...\n");
    })

    SCM_TRUE = (scmObject)malloc(sizeof(scmObject));
    SCM_FALSE = (scmObject)malloc(sizeof(scmObject));
    SCM_NULL = (scmObject)malloc(sizeof(scmObject));
    SCM_INV = (scmObject)malloc(sizeof(scmObject));

    initializeSymbolTableBuffer();

    SCM_TRUE->tag = TAG_TRUE;
    SCM_TRUE->value.scmInt = 1;

    SCM_FALSE->tag = TAG_FALSE;
    SCM_FALSE->value.scmInt = 0;

    SCM_NULL->tag = TAG_NULL;
    SCM_NULL->value.scmInt = 0;

    SCM_INV->tag = TAG_INV;
    SCM_INV->value.scmInt = 0;
}

int main(int argCount, char *argValues[])
{
    printf("Welcome to my scheme 2021!!\n");

    MAIN_DEBUG_CODE({
        printf("%d\n", argCount);
        printf("%s\n", argValues[1]);
    })

    scm_initialize();
    selftest();

    // Read Eval Print Loop
    scmObject expr;
    for (;;)
    {
        if (setjmp(savebuf) == 0)
        {
            // Vergleichbar mit try-Block
            printf("\n>");
            expr = scm_read();
            expr = scm_eval(expr);
            scm_print(expr);
        }
        else
        {
            printf("SET JUMP WURDE AUFGEFURFEN\n");
        }
    }

    return 0;
}