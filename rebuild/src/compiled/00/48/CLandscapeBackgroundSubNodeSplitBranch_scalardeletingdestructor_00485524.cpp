#pragma optimize("s",on)
#include "rebuild_abi.h"

// CLandscapeBackgroundSubNodeSplitBranch::`scalar deleting destructor' @ 0x00485524
// module: CLandscapeBackgroundSubNodeSplitBranch  (thiscall, void*, 2 params)
// VC7.1, x86, /O2 /Oy.
//
// OVER-CAPTURE: the 71-byte retail slice packs FOUR contiguous routines with no
// inter-function padding.  Only the LEADING 43-byte (0x00..0x2a, ret 4) scalar
// deleting destructor is a member of this class; it is reconstructed here.  The
// three trailing orphans (a getter, a member-deleting helper, a copy-out helper)
// are separate functions and are intentionally NOT reproduced.  real_len = 43.
//
// Retail idiom (inlined non-deleting destructor + conditional operator delete):
//   push esi; mov esi,ecx                ; self = this (ecx)
//   lea ecx,[esi+8]                      ; arg = base subobject at this+8
//   mov dword ptr [esi],DERIVED_VT       ; stamp this class's vptr
//   call <base-subobject dtor>           ; destruct base/member at this+8
//   test byte ptr [esp+8],1              ; flags & 1 ?  (scheduled early)
//   mov dword ptr [esi],BASE_VT          ; restore base class vptr
//   je skip; push esi; call operator delete; pop ecx
// skip:
//   mov eax,esi; pop esi; ret 4          ; return this
//
// Both vtable stores are absolute-immediate writes (`mov [esi],imm32`) and the
// call rel32 targets are relocation-masked in parity, so the concrete symbol /
// vtable addresses do not affect the byte match.  The sub-object destructor at
// +8 is __fastcall (ecx = this+8); modeled as a free __fastcall(objptr).  The
// deleting-destructor itself takes ecx = this and pops its one flags argument
// (ret 4) -- modeled __fastcall with a trailing stack `flags` parameter.

struct CLandscapeBackgroundSubNodeSplitBranch;
struct SplitBranchBaseSubObject;

// base-subobject destructor at this+8 (thiscall / ecx = subobject)
extern "C" void FABLE_FASTCALL SplitBranch_BaseSubObject_dtor(
        SplitBranchBaseSubObject* thisptr);

extern "C" void FABLE_CDECL fable_operator_delete(void* p);

// Relocation-masked absolute vtable addresses stamped by the inlined destructor.
static const fable_u32 kSplitBranchDerivedVtable = 0x01236794u;
static const fable_u32 kSplitBranchBaseVtable    = 0x0122f83cu;

extern "C" void* FABLE_FASTCALL CLandscapeBackgroundSubNodeSplitBranch_scalar_deleting_dtor(
        CLandscapeBackgroundSubNodeSplitBranch* thisptr,
        int /*edx*/,
        unsigned int flags)
{
    fable_u32* vfptr = reinterpret_cast<fable_u32*>(thisptr);

    // stamp this class's vtable, then destruct the base subobject at this+8
    *vfptr = kSplitBranchDerivedVtable;
    SplitBranch_BaseSubObject_dtor(
        reinterpret_cast<SplitBranchBaseSubObject*>(
            reinterpret_cast<fable_u8*>(thisptr) + 8));

    // restore the base class vtable
    *vfptr = kSplitBranchBaseVtable;

    if (flags & 1)
        fable_operator_delete(thisptr);

    return thisptr;
}