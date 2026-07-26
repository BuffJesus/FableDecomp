#include <stdio.h>
#include <wchar.h>

#include "fable_filesystem.h"
#include "fable_gfmain.h"

int main()
{
    g_FableConsoleVariablesBoundaryCalls = 0;

    char commandLine[] = "";
    const long result = FableRunGFMainPhase1(commandLine);
    if (result != 0)
    {
        printf("FABLETLC_GFMAIN_PHASE1_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_FableConsoleVariablesBoundaryCalls != 1)
    {
        printf("FABLETLC_GFMAIN_PHASE1_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    if (wcslen(g_CAFileCurrentPath_013BCA10) == 0)
    {
        printf("FABLETLC_GFMAIN_PHASE1_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS\n");
    return 0;
}
