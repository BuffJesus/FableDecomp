#include <stdio.h>

extern bool g_013b8604, g_013b8607, g_013b8608, g_013b860b;
extern bool g_FableRunFromDVD_013B8615, GUseRetailBanks, g_013b861b, g_013b861c, g_013b861d;
extern bool g_013b861e, g_FablePresentationIntervalOverride_013B8628, g_013b8634, g_013b8648, g_013b864f;
extern unsigned char g_FableStartMainGame_013B8605;
extern long g_013b8624;

int main()
{
    bool* flags[] = {
        &g_013b8604, &g_013b8607, &g_013b8608,
        &g_013b860b, &g_FableRunFromDVD_013B8615, &GUseRetailBanks, &g_013b861b,
        &g_013b861c, &g_013b861d, &g_013b861e, &g_FablePresentationIntervalOverride_013B8628,
        &g_013b8634, &g_013b8648, &g_013b864f
    };
    for (unsigned i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i)
        if (*flags[i]) return 1;
    if (g_013b8624 != 0 || g_FableStartMainGame_013B8605 != 0) return 2;
    for (unsigned round = 0; round < 16; ++round) {
        for (unsigned i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i)
            *flags[i] = ((round + i) & 1) != 0;
        g_FableStartMainGame_013B8605 = (unsigned char)(round + 1);
        g_013b8624 = round * 71;
        for (unsigned i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i)
            if (*flags[i] != (((round + i) & 1) != 0)) return 3;
        if (g_FableStartMainGame_013B8605 != round + 1) return 5;
        if (g_013b8624 != static_cast<long>(round * 71)) return 4;
    }
    puts("GFMAIN_RUNTIME_FLAGS_PASS cases=16");
    return 0;
}
