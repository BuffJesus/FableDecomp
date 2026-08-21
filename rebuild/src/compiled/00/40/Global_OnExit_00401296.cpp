#pragma optimize("s",on)
// __onexit @ 00401296 -- the CRT's onexit registrar.
//
// Two things pin the retail bytes:
//  * it RETURNS the result, so the `_onexit` path is a TAIL JUMP, and the jump goes
//    through the IMPORT table (`jmp dword ptr [__imp__onexit]`) -- the CRT routine it
//    reaches is the msvcr71 export, not a statically linked copy. Modelling it as a
//    void call gives `call; ret` and never matches.
//  * `#pragma optimize("s",on)` is required for the argument to be pushed straight from
//    the stack (`push dword ptr [esp+0x0c]`); the speed setting hoists it into ecx first,
//    which is both longer and in the wrong place.

#include <stdlib.h>

extern "C" __declspec(dllimport) _onexit_t __cdecl CrtOnExit(_onexit_t function);
extern "C" _onexit_t __cdecl __dllonexit(
    _onexit_t function, _onexit_t** first, _onexit_t** last);
extern "C" _onexit_t* DAT_0143e990;
extern "C" _onexit_t* DAT_0143e9a0;

extern "C" _onexit_t __cdecl __onexit(_onexit_t function)
{
    if (DAT_0143e9a0 == reinterpret_cast<_onexit_t*>(-1))
        return CrtOnExit(function);
    return __dllonexit(function, &DAT_0143e9a0, &DAT_0143e990);
}
