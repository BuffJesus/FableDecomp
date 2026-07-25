#pragma optimize("s",on)
#include "rebuild_abi.h"

// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x0046f483
//
// Standard MSVC vector-deleting-destructor thunk. Retail:
//   push esi ; mov esi,ecx        ; esi = this
//   call <scalar destructor>      ; this in ecx (thiscall)
//   test [esp+8],1                ; (flags & 1) -> should free backing block
//   je  skip
//   push esi ; call operator delete ; pop ecx
//   mov eax,esi ; pop esi ; ret 4
//
// Modeled __fastcall so 'this' arrives in ecx exactly as the thiscall passes it;
// the second (edx) slot is an unused placeholder, the 'flags' word is the single
// stack parameter (at [esp+8] after the push esi), matching the retail test.

struct CLandscapeBackgroundPatch
{
    fable_u32 vtbl; // this+0x00 (layout inferred; only 'this' is used)
};

// Scalar (ordinary) destructor; thisptr in ecx -> modeled __fastcall.
extern void FABLE_FASTCALL CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);

// operator delete (engine allocator free), __cdecl argument pushed.
extern void FABLE_CDECL CLandscapeBackgroundPatch_opDelete(void* block);

void* FABLE_FASTCALL CLandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int edx_unused, fable_u32 flags)
{
    (void)edx_unused;
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1)
    {
        CLandscapeBackgroundPatch_opDelete(thisptr);
    }
    return thisptr;
}