#include <float.h>

// Retail's CRT startup object for __setdefaultprecision was compiled favoring
// size, so MSVC13.10 discards the two pushed args with `pop ecx; pop ecx`
// (2 bytes) instead of `add esp,8` (3 bytes). The parity harness pins
// `/O2 /Oy` (favor speed), which otherwise emits `add esp,8`. Forcing the
// size-favoring optimizer for this translation unit reproduces the exact
// retail cleanup idiom while keeping identical behavior.
#pragma optimize("s", on)

extern "C" void __cdecl __setdefaultprecision()
{
    _controlfp(0x10000, 0x30000);
}
