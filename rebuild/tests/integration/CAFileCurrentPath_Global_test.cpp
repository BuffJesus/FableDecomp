#include <stdio.h>
#include "fable_filesystem.h"

static int constructed;
static int destroyed;
CWideString::CWideString() : storage_(0) { ++constructed; }
CWideString::~CWideString() { ++destroyed; }

int main()
{
    if (sizeof(g_CAFileCurrentPath_013BCA10) != 4 ||
        constructed != 1 || destroyed != 0)
        return 1;
    if (*reinterpret_cast<void**>(&g_CAFileCurrentPath_013BCA10) != 0)
        return 2;
    puts("CAFILE_CURRENT_PATH_GLOBAL_PASS");
    return 0;
}
