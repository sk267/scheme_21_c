#include <stdio.h>
#include "scheme.h"

char parkedChar = '\0';
int nextChar()
{
    int nextChar;
    if (parkedChar != '\0')
    {
        char tmp = parkedChar;
        parkedChar = '\0';
        return tmp;
    }
    else
    {
        nextChar = fgetc(stdin);
        if (nextChar < 0)
        {
            return -1;
        }
        else
        {
            return nextChar;
        }
    }
}

void unreadChar(char input)
{
    parkedChar = input;
}
