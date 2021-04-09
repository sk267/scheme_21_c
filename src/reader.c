#include "scheme.h"

// #define READER_DEBUG

#ifdef READER_DEBUG
#define READER_DEBUG_CODE(code) code
#else
#define READER_DEBUG_CODE(code) /*as nothing*/
#endif

int charToInt(char input)
{

    int out;
    out = input - '0';
    return out;
}

void skipWhitespace()
{
    char actChar;
    do
    {
        actChar = nextChar();
    } while (
        (actChar == ' ') || (actChar == '\n') || (actChar == '\t'));

    unreadChar(actChar);
}

char actChar;

scmObject read_Integer(scmObject newObj)
{
    // alle Integer sammeln
    int number = charToInt(actChar);
    while (true)
    {
        actChar = nextChar();
        if ((actChar >= '0') && (actChar <= '9'))
        {
            number *= 10;
            number += charToInt(actChar);
            READER_DEBUG_CODE({
                printf("scm_read: number is now: %d\n", number);
            })
        }
        else
        {
            break;
        }
    }

    newObj = newInteger(number);

    READER_DEBUG_CODE({
        printf("Hallo \n");
        printf("read_Integer: \ntag: %d\nvalue: %d\n", newObj->tag, newObj->value.scmInt);
    })
    return newObj;
}

scmObject read_String(scmObject newObj)
{

    int buffer_capacity = 4;
    char *charBuffer = (char *)malloc(buffer_capacity);
    int i = 0;

    while (true)
    {
        actChar = nextChar();
        if (actChar != '"')
        {
            charBuffer[i] = actChar;
            READER_DEBUG_CODE({
                printf("read_String: charBuffer: %s\n", charBuffer);
                printf("read_string: actChar: %c\n", actChar);
            })
        }
        else
        {
            break;
        }

        i++;
        // Ensure that the charBuffer is big enough
        if ((i + 2) > buffer_capacity)
        {
            // Enlarge charBuffer
            buffer_capacity *= 1.8;
            char *newBuffer = (char *)realloc(charBuffer, buffer_capacity);
            charBuffer = newBuffer;
        }
    }

    newObj = newString(charBuffer);
    free(charBuffer);

    return newObj;
}

bool isValidSymbolChar(char input)
{
    if (((input >= 'A') && (input >= 'A')) || ((input >= '0') && (input <= '9')))
    {
        return true;
    }
    else
    {
        return false;
    }
}

typedef struct charBufferStruct charBuffer;

struct charBufferStruct
{
    char *characters;
    int bufferSize;
    int nCharsInBuffer;
};

void init_char_buffer(charBuffer *b)
{
    b->characters = malloc(4);
    b->bufferSize = 4;
    b->nCharsInBuffer = 0;
}

void add_to_char_buffer(charBuffer *b, char charToAdd)
{
    // Ist Buffer schon am Anschlag?
    if (b->nCharsInBuffer == b->bufferSize)
    {
        // Buffer vergroessern:
        READER_DEBUG_CODE({
            printf("Buffer wird jetzt vergroessert!\n");
        })
        b->bufferSize *= 1.8;
        b->characters = realloc(b->characters, b->bufferSize);
    }

    b->characters[b->nCharsInBuffer] = charToAdd;
    b->nCharsInBuffer++;
}

scmObject read_Symbol(scmObject newObj)
{

    charBuffer buffer;
    init_char_buffer(&buffer);

    while (true)
    {
        actChar = nextChar();
        if (isValidSymbolChar(actChar) == true)
        {
            add_to_char_buffer(&buffer, actChar);
            READER_DEBUG_CODE({
                printf("read_Symbol: charBuffer: %s\n", buffer.characters);
                printf("read_Symbol: actChar: %c\n", actChar);
            })
        }
        else
        {
            break;
        }
    }

    newObj = newSymbol(buffer.characters, buffer.nCharsInBuffer);
    // free(charBuffer);

    return newObj;
}

scmObject scm_read()
{
    scmObject newObj = (scmObject)malloc(sizeof(scmObject));

    printf(">");
    READER_DEBUG_CODE({
        printf("Betrete scm_read! \n");
    })

    skipWhitespace();
    actChar = nextChar();
    READER_DEBUG_CODE({
        printf("----------------------------scm_read> actChar: %d \n", actChar);
    })

    if ((actChar >= 48) && (actChar <= 75))
    {
        // INTEGER #################################################
        newObj = read_Integer(newObj);
    }
    else if (actChar == '"')
    {
        // STRING ##################################################
        newObj = read_String(newObj);
    }
    else if ((actChar >= 'A') && (actChar <= 'z'))
    {
        // SYMBOL ##################################################
        unreadChar(actChar);
        newObj = read_Symbol(newObj);
    }
    else if (actChar == '#')
    {
        actChar = nextChar();
        if (actChar == 't')
        {
            // TRUE ################################################
            return SCM_TRUE;
        }
        if (actChar == 'f')
        {
            // FALSE ################################################
            return SCM_FALSE;
        }
    }
    else if (actChar == '(')
    {
        actChar = nextChar();
        if (actChar == ')')
        {
            // NULL ################################################
            return SCM_NULL;
        }
    }
    else
    {
        printf("scm_read: noch nicht implementiert\n");
    }

    return newObj;
}
