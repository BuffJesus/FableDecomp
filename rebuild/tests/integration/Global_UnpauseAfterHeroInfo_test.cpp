#include "fable_world.h"
#include <stdio.h>

int main()
{
    if (!GUnpauseAfterClosingHeroInformationScreen) return 1;
    int cases = 0;
    for (int i = 0; i != 8; ++i)
    {
        GUnpauseAfterClosingHeroInformationScreen = (i & 1) != 0;
        if (GUnpauseAfterClosingHeroInformationScreen != ((i & 1) != 0)) return 2;
        ++cases;
    }
    GUnpauseAfterClosingHeroInformationScreen = true;
    printf("UNPAUSE_HERO_INFO_GLOBAL_PASS cases=%d\n", cases);
    return 0;
}
