#include "scheme.h"

// #define READER_DEBUG

#ifdef READER_DEBUG
#define READER_DEBUG_CODE(code) code
#else
#define READER_DEBUG_CODE(code) /*as nothing*/
#endif
int actChar;

int charToInt(char input)
{

    int out;
    out = input - '0';
    return out;
}

void skipWhitespace()
{
    do
    {
        actChar = nextChar();
        READER_DEBUG_CODE({
            printf("-----------------skipWhitespace: actChar: %c\n", (int)actChar);
        })
    } while (
        (actChar == ' ') || (actChar == '\n') || (actChar == '\t') || (actChar == '\r'));

    unreadChar(actChar);
    READER_DEBUG_CODE({
        printf("-----------------skipWhitespace: noch da \n");
    })
}

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
            unreadChar(actChar);
            break;
        }
    }

    newObj = newInteger(number);

    READER_DEBUG_CODE({
        printf("read_Integer: \ntag: %d\nvalue: %d\n", newObj->tag, newObj->value.scmInt);
    })
    return newObj;
}

bool isValidSymbolChar(char input)
{
    if (
        ((input >= 'A') && (input <= 'Z')) ||
        ((input >= 'a') && (input <= 'z')) ||
        ((input >= '0') && (input <= '9')) ||
        ((input >= '*') && (input <= '/')) ||
        (input == '<') ||
        (input == '=') ||
        (input == '>') ||
        (input == '?') ||
        (input == '!'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isValidStringChar(char input)
{
    if (
        ((input >= 'A') && (input <= 'Z')) ||
        ((input >= 'a') && (input <= 'z')) ||
        ((input >= '0') && (input <= '9')) ||
        ((input >= '*') && (input <= '/')) ||
        ((input >= '<') && (input <= '>')) ||
        (input == '!') ||
        (input == '-') || (input == ' ') || (input == '_'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// bool isValidStringChar(char input)
// {
//     if (input != '\0')
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

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

void free_char_buffer(charBuffer *b)
{
    READER_DEBUG_CODE({
        printf("-------------------------------free_char_buffer betreten\n");
    })
    free(b->characters);
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
    READER_DEBUG_CODE(
        {
            printf("--------------------------------------read_Symbol: eben gerade init_char_buffer aufgerufen...\n");
            printf("--------------------------------------read_Symbol: buffer.characters: %s\n", buffer.characters);
        })

    while (true)
    {
        actChar = nextChar();
        if (isValidSymbolChar(actChar) == true)
        {
            add_to_char_buffer(&buffer, actChar);
            READER_DEBUG_CODE(
                {
                    printf("read_Symbol: actChar: %c\n", actChar);
                    printf("read_Symbol: charBuffer: %s\n", buffer.characters);
                })
        }
        else
        {
            unreadChar(actChar);
            add_to_char_buffer(&buffer, '\0');
            break;
        }
    }

    READER_DEBUG_CODE({
        printf("read_Symbol: buffer.nCharsInBuffer: %d\n", buffer.nCharsInBuffer);
    })

    newObj = newSymbol(buffer.characters, buffer.nCharsInBuffer);
    free_char_buffer(&buffer);
    return newObj;
}

scmObject read_String(scmObject newObj)
{
    READER_DEBUG_CODE({
        printf("read String betreten \n");
    })
    charBuffer buffer;
    init_char_buffer(&buffer);

    while (true)
    {
        actChar = nextChar();

        if (actChar == 42)
        {
            // " entdeckt, rausgehen!
            break;
        }
        if (isValidStringChar(actChar) == true)
        {
            add_to_char_buffer(&buffer, actChar);
            READER_DEBUG_CODE(
                {
                    printf("read_Symbol: charBuffer: %s\n", buffer.characters);
                    printf("read_Symbol: actChar: %c\n", actChar);
                })
        }
        else
        {
            add_to_char_buffer(&buffer, '\0');
            break;
        }
    }

    newObj = newString(buffer.characters, buffer.nCharsInBuffer);
    free_char_buffer(&buffer);

    return newObj;
}

scmObject read_Cons(scmObject nextCar)
{

    READER_DEBUG_CODE({
        printf("Betrete read_Cons\n");
    })
    scmObject car = (scmObject)malloc(sizeof(struct scmObjectStruct));
    scmObject cdr;

    car = nextCar;
    cdr = scm_read();

    if (getTag(cdr) == TAG_INV)
    {
        // cdr = SCM_NULL;
        return newCons(car, SCM_NULL);
    }
    else
    {
        return newCons(car, read_Cons(cdr));
    }
}

scmObject scm_read()
{
    scmObject newObj = (scmObject)malloc(sizeof(scmObject));

    // printf("\n>");
    READER_DEBUG_CODE({
        printf("Betrete scm_read! \n");
    })

    skipWhitespace();
    actChar = nextChar();
    // printf("scm_read> actChar: %d \n", (int)actChar);
    READER_DEBUG_CODE({
        printf("scm_read> actChar: %c \n", actChar);
    })

    if (actChar == -1)
    {
        // printf("------------------------------ EOF gefunden!! \n");
        return SCM_FALSE;
    }

    if (actChar == ')')
    {
        return SCM_INV;
    }
    if ((actChar >= 48) && (actChar <= 57))
    {
        // INTEGER #################################################
        // printf("scm_reader: isInteger -> true\n");
        newObj = read_Integer(newObj);
    }
    else if (actChar == '"')
    {
        // STRING ##################################################
        newObj = read_String(newObj);
    }
    else if (isValidSymbolChar(actChar))
    {
        // SYMBOL ##################################################
        // printf("scm_reader: isValidSymbolChar -> true\n");
        unreadChar(actChar);
        newObj = read_Symbol(newObj);
    }
    else if (actChar == '#')
    {
        actChar = nextChar();
        if (actChar == 't')
        {
            // TRUE ################################################
            printf("true erkannt! \n");
            return SCM_TRUE;
        }
        else if (actChar == 'f')
        {
            // FALSE ################################################
            printf("reader: false betreten! \n");
            return SCM_FALSE;
        }
        else
        {
            scmError("#t or #f are possible, nothing else!");
        }
    }
    else if (actChar == '(')
    {
        skipWhitespace();
        actChar = nextChar();
        unreadChar(actChar);
        if (actChar == ')')
        {
            return SCM_NULL;
        }
        else
        {
            // CONS ######################################################
            newObj = read_Cons(scm_read());
        }
    }
    else
    {
        printf("scm_read: noch nicht implementiert\n");
    }

    return newObj;
}
