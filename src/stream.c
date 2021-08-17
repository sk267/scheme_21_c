#include "scheme.h"

char parkedChar = '\0';

int SINGLE;

static int nextCharFromConsole()
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
    return 0;
}

static int nextCharFromFile()
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
        nextChar = getc(FILE_POINTER);

        if (nextChar < 0)
        {
            switchToReadFromConsole();
            SINGLE = -1;
            return -1;
        }
        else if (nextChar == EOF)
        {
            switchToReadFromConsole();
            return -1;
        }
        else
        {
            SINGLE = nextChar;
            return nextChar;
        }
    }
    return 0;
}

int nextChar()
{
    if (WHICH_READ_V->readFromConsole)
    {
        return nextCharFromConsole();
    }
    if (WHICH_READ_V->readFromFile)
    {
        return nextCharFromFile();
    }

    return 0;
}

void unreadChar(char input)
{
    parkedChar = input;
}
