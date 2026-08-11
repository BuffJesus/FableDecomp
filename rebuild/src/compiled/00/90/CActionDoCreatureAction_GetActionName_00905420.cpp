#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x00905420
// Retail: __fastcall member of CActionDoCreatureAction returning a CCharString
// BY VALUE. ecx = this (CActionDoCreatureAction, unused); the hidden return-buffer
// pointer is the first stack arg. Modeled as a free __fastcall so ecx (this) and
// edx are consumed/ignored and the return buffer arrives as the first stack slot
// ([esp+4], loaded into esi after push esi -> [esp+8]). Body constructs the
// CCharString from a relocated string constant with length -1 and returns the
// buffer ptr in eax.
// Shape: push esi; mov esi,[esp+8]; push -1; push STR; mov ecx,esi; call ctor;
//        mov eax,esi; pop esi; ret 4.

// CCharString ctor at 0x99ebf0: __fastcall (this in ecx), two callee-cleaned stack
// args (const char* lit, int len). Model as a real member so a genuine thiscall
// call site is emitted (ecx=this, edx untouched).
struct CCharString {
    int dummy;
    CCharString* ctor_99ebf0(const char* lit, int len);
};

extern "C" CCharString* FABLE_FASTCALL
FableGetActionName_00905420(void* thisptr, void* edx, CCharString* result)
{
    result->ctor_99ebf0((const char*)0x128d904, -1);
    return result;
}