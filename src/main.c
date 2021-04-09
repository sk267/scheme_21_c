#include "scheme.h"

scmObject SCM_TRUE;
scmObject SCM_FALSE;
scmObject SCM_NULL;

static void
scm_initialize()
{
    printf("initialize...\n");

    SCM_TRUE = (scmObject)malloc(sizeof(scmObject));
    SCM_FALSE = (scmObject)malloc(sizeof(scmObject));
    SCM_NULL = (scmObject)malloc(sizeof(scmObject));

    SCM_TRUE->tag = TAG_TRUE;

    SCM_TRUE->value.scmInt = 1;

    SCM_FALSE->tag = TAG_FALSE;
    SCM_FALSE->value.scmInt = 0;

    SCM_NULL->tag = TAG_NULL;
    SCM_NULL->value.scmInt = 0;
}

int main(int argCount, char *argValues[])
{
    printf("Welcome to my scheme 2021!!\n");
    printf("%d\n", argCount);
    printf("%s\n", argValues[1]);

    scm_initialize();
    selftest();

    // Read Eval Print Loop
    scmObject expr;

    for (;;)
    {
        expr = scm_read();
        expr = scm_eval(expr);
        scm_print(expr);
    }

    return 0;
}