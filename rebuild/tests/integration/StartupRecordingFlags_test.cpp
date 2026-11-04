#include "fable_game.h"
#include <stdio.h>

extern bool g_FableUsePhysicalDVD_013B85F2;
extern bool g_FableSaveInputEvents_013B85F5;
extern bool g_FableLoadInputEvents_013B85F6;

int main()
{
    bool* flags[] = {
        &g_FableUsePhysicalDVD_013B85F2,
        &g_FableSaveInputEvents_013B85F5,
        &g_FableLoadInputEvents_013B85F6,
        &g_SaveEventPackages_013B85F7,
        &g_LoadEventPackages_013B85F8
    };
    for (int j = 0; j < 5; ++j) if (*flags[j]) return 1;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 5; ++j) *flags[j] = ((i + j) & 1) != 0;
        for (int j = 0; j < 5; ++j) if (*flags[j] != (((i + j) & 1) != 0)) return 2;
    }
    puts("STARTUP_RECORDING_FLAGS_PASS cases=16");
    return 0;
}
