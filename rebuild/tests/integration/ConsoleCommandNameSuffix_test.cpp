#include "fable_string.h"

#include <stdio.h>
#include <string.h>

extern const CCharString g_013caa48;

static const CCharString* constructed;
static const char* literal;
static long length;
static int calls;

CCharString::CCharString(const char* text, long count)
{
    constructed = this;
    literal = text;
    length = count;
    ++calls;
}

CCharString::~CCharString() {}

int main()
{
    if (calls != 1 || constructed != &g_013caa48 ||
        !literal || strcmp(literal, "(") != 0 || length != -1)
        return 1;
    puts("CONSOLE_COMMAND_SUFFIX_OK");
    return 0;
}
