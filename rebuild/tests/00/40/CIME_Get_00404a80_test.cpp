#include "fable_ime.h"
#include <stdio.h>
#include <string.h>

NUISystem::CIME g_FableIME_013B7CD8;

int main()
{
    // The accessor must return the owner itself without initializing or
    // changing it. Exercise nonzero storage to expose accidental resets.
    unsigned char expected[sizeof(NUISystem::CIME)];
    memset(expected, 0x5a, sizeof(expected));
    memcpy(&g_FableIME_013B7CD8, expected, sizeof(expected));
    for (int i = 0; i != 3; ++i)
        if (&NUISystem::CIME::Get() != &g_FableIME_013B7CD8 ||
            memcmp(&g_FableIME_013B7CD8, expected, sizeof(expected)))
            return 1;
    puts("CIME_GET_00404a80_PASS");
    return 0;
}
