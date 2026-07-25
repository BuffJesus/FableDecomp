#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x004664ff  CLandscapeBackgroundPatch::`vector deleting destructor'
// Standard MSVC vector-deleting-destructor thunk.
//   this in ecx, flags on stack (bit0 => operator delete(this)).
// Modeled as a free __fastcall(this) function (byte-identical to __fastcall).

struct CLandscapeBackgroundPatch;

// scalar/base destructor (this-only) at 0x596051
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);
// operator delete at 0x7984bd
extern "C" void __cdecl fable_operator_delete(void* p);

extern "C" void* __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1)
    {
        fable_operator_delete(thisptr);
    }
    return thisptr;
}