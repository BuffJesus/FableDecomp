#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x004584e8  CLandscapeBackgroundPatch `vector_deleting_destructor'
// Standard MSVC vector deleting destructor:
//   push esi; mov esi,ecx; call <scalar dtor>; test [esp+8],1;
//   if set: push esi; call operator delete; pop ecx;
//   mov eax,esi; pop esi; ret 4
//
// __fastcall(this, unsigned flags) modeled as __fastcall(this, edx_dummy, flags).

struct CLandscapeBackgroundPatch;

// scalar destructor called first (relocation-masked target)
extern void FABLE_FASTCALL CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);
// operator delete (relocation-masked target)
extern void FABLE_CDECL fable_operator_delete(void* p);

void* FABLE_FASTCALL CLandscapeBackgroundPatch_vector_dtor(
    CLandscapeBackgroundPatch* thisptr, int edxDummy, fable_u32 flags)
{
    (void)edxDummy;
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1)
        fable_operator_delete(thisptr);
    return thisptr;
}