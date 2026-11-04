#pragma once
// lib_global_tools: the engine's linear-congruential generator. GFROR13 is
// out of line in retail (0x00497890); GFRandom is inline everywhere it is
// used (FableWin.exe ?GFRandom@@YIKKPAK@Z, 018CF370, is the body here).
unsigned long __fastcall GFROR13(unsigned long value);

inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
