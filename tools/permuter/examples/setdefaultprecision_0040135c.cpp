#include <float.h>

// Choice point: which optimizer bias reproduces retail's `pop ecx; pop ecx`
// stack cleanup instead of `add esp,8`. The permuter will discover the answer.
PERMPRAGMA(#pragma optimize("s", on)|#pragma optimize("t", on)|)

extern "C" void __cdecl __setdefaultprecision()
{
    _controlfp(0x10000, 0x30000);
}
