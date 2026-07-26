// Std_Move_Backward @ 0x00405ba0  (VC7.1 /O2 /Oy) -- RELOCATION_MATCH (call rel32 masked)
// Was previously on the PARITY_TRIAGE do-not-regrind list as an "unmatchable scheduler
// artifact". Cracked via LIVENESS-SHAPING: expressing the count (last-first) as a REPEATED
// INLINE subexpression instead of a named local forces MSVC to keep it live in esi with
// retail's exact `mov esi,edx; sub esi,ecx` ordering (the named-local form emits `sub;mov`).
#include <string.h>

void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        dest = (char*)memmove(dest, first, (char*)last - (char*)first) + ((char*)last - (char*)first);
    }
    return dest;
}
