#pragma optimize("s",on)
#include "rebuild_abi.h"

/* 0x0046f760  CLandscapeBackgroundPatch::`vector deleting destructor'
 * Standard MSVC vector-deleting-destructor thunk:
 *   push esi; mov esi,ecx
 *   call <scalar destructor>          ; this in ecx
 *   test [esp+8], 1                   ; flags & 1 ?
 *   if set: push esi; call operator delete; pop ecx
 *   mov eax,esi; pop esi; ret 4
 * Modeled __fastcall(this, edx, flags) == __fastcall(this, flags).
 */

struct CLandscapeBackgroundPatch;

/* engine callees (addresses relocation-masked) */
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);
extern "C" void __cdecl operator_delete_thunk(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
        CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1)
        operator_delete_thunk(thisptr);
    return thisptr;
}