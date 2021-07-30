#include "scheme.h"

void scmAssertVerbose(int ok, char *msg, char *file, int line)
{
    if (!ok)
    {
        printf("\n!!!!!!!!!\tERROR\t!!!!!!!!! at %s line %d: ", file, line);
        printf(msg);
        printf("\n");
        // longjmp(savebuf, 1);
    }
}

void scmErrorVerbose(char *msg, char *file, int line)
{
    printf("\n!!!!!!!!ERROR\t!!!!!!!! ----------------- at %s line %d: ", file, line);
    printf(msg);
    printf("\n");
    longjmp(savebuf, 1);
}