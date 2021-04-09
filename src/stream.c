#include <stdio.h>

char parkedChar = '\0';
char nextChar()
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
        nextChar = getchar();
        return (char)nextChar;
    }
}

void unreadChar(char input)
{
    parkedChar = input;
}
