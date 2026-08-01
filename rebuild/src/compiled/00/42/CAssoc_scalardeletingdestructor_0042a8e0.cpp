#pragma optimize("s",on)
#include "rebuild_abi.h"

// CAssoc::`scalar deleting destructor' @ 0x0042a8e0
// module: CAssoc   (thiscall, returns void*, 2 params)
// VC7.1, x86, /O2 /Oy.
//
// OVER-CAPTURE: the 100-byte retail slice packs SEVEN contiguous routines with
// NO inter-function padding (no int3 between them).  Only the LEADING 28-byte
// (0x00..0x1b, `ret 4`) scalar deleting destructor belongs to CAssoc; it is the
// only function reconstructed here.  real_len = 28.  The six trailing orphans
//   0x1c  mov eax,[esp+4]; add eax,0x10; ret          (a +0x10 getter)
//   0x24  mov eax,[esp+4]; add eax,0x10; ret          (identical getter copy)
//   0x2c  test ecx,ecx; je .; push edx; call rel32; ret (guarded notify)
//   0x37  test ecx,ecx; je .; copy 8 bytes edx->ecx; ret (guarded copy-out)
//   0x46  … identical guarded copy-out
//   0x55  … identical guarded copy-out
// are separate functions and are intentionally NOT reproduced.
//
// Retail idiom (canonical MSVC scalar-deleting-destructor with an inlined
// non-deleting destructor call):
//   push esi; mov esi,ecx        ; self = this (ecx), kept in a callee-saved reg
//   call <CAssoc::~CAssoc>       ; ecx still = this -> non-deleting dtor (thiscall)
//   test byte ptr [esp+8],1      ; flags & 1 ?
//   je  skip
//   push esi; call operator delete; pop ecx
// skip:
//   mov eax,esi; pop esi; ret 4  ; return this
//
// The non-deleting destructor is thiscall (ecx = this); because `this` is still
// live in ecx right after `mov esi,ecx`, it is modeled as a free __fastcall that
// receives the object pointer in ecx.  operator delete is the ordinary __cdecl
// global.  Both call rel32 targets are relocation-masked in parity, so the
// concrete symbol addresses do not affect the byte match.  The deleting
// destructor takes ecx = this and pops its single flags dword (`ret 4`); it is
// modeled __fastcall with a trailing stack `flags` parameter and returns this.

struct CAssoc;

// Non-deleting destructor CAssoc::~CAssoc (thiscall / ecx = this), modeled as a
// free __fastcall that takes the object pointer in ecx.
extern "C" void FABLE_FASTCALL CAssoc_dtor(CAssoc* thisptr);

// Global scalar operator delete (__cdecl).
extern "C" void FABLE_CDECL fable_operator_delete(void* p);

extern "C" void* FABLE_FASTCALL CAssoc_scalar_deleting_dtor(
        CAssoc* thisptr,
        int /*edx*/,
        unsigned int flags)
{
    CAssoc_dtor(thisptr);

    if (flags & 1)
        fable_operator_delete(thisptr);

    return thisptr;
}