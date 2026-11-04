#include <stdio.h>
#include "fable_string.h"

extern CWideString g_013ca7dc;
static int constructed;
static int destroyed;
CWideString::CWideString() : storage_(0) { ++constructed; }
CWideString::~CWideString() { ++destroyed; }

int main()
{
    if (sizeof(g_013ca7dc) != 4 || constructed != 1 || destroyed != 0)
        return 1;
    if (*reinterpret_cast<void**>(&g_013ca7dc) != 0) return 2;
    puts("DEFINITION_PATH_GLOBAL_PASS");
    return 0;
}
