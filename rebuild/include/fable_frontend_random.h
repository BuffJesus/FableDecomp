#pragma once

#include "rebuild_abi.h"
#include <stddef.h>

extern "C" __declspec(dllimport) void __stdcall OutputDebugStringW(const wchar_t*);

// Functional extraction of GFRandomNoRepeat (005472A0) and GFROR13 (00497890).
// Keep the seed owned by each swapping component; Initialise (00547360) sets 13.
inline fable_u32 FableFrontendRandomFrame(fable_u32 count, fable_u32 previous,
    fable_u32& seed)
{
    unsigned retries = 0;
    for (;;)
    {
        seed = seed * 0x24A1u + 0x24DFu;
        seed = (seed >> 13) | (seed << 19);
        const fable_u32 selected = count ? seed % count : 0;
        // Retail performs the initial draw plus at most 101 retries. On the
        // last retry it emits its diagnostic even if that draw is different.
        if (retries > 100)
        {
            OutputDebugStringW(L"GFRandomNoRepeat is going on a bit - breaking the loop");
            return selected;
        }
        if (count <= 1 || selected != previous)
            return selected;
        ++retries;
    }
}
