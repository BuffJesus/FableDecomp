#include "fable_game.h"
#include <stdio.h>

extern bool g_FableUseCompiledGlobalThings_013B8609;
extern bool g_FableUseCompiledAnimationEvents_013B860A;
extern bool g_FableUseCompiledDefs_013B8617;
extern bool g_FableUseCompiledWorldFiles_013B8618;
extern bool g_FableUseCompiledSoundSymbols_013B8619;

int main()
{
    bool* flags[] = {
        &g_FableUseCompiledGlobalThings_013B8609,
        &g_FableUseCompiledAnimationEvents_013B860A,
        &g_FableUseCompiledDefs_013B8617,
        &g_FableUseCompiledWorldFiles_013B8618,
        &g_FableUseCompiledSoundSymbols_013B8619
    };
    for (int j = 0; j < 5; ++j) if (*flags[j]) return 1;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 5; ++j) *flags[j] = ((i >> (j & 3)) & 1) != 0;
        for (int j = 0; j < 5; ++j)
            if (*flags[j] != (((i >> (j & 3)) & 1) != 0)) return 2;
    }
    puts("COMPILED_RESOURCE_FLAGS_PASS cases=16");
    return 0;
}
