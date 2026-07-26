#pragma optimize("s",on)
#include "rebuild_abi.h"

// CEngineSubPrimitiveText::`scalar deleting destructor' @ 0x0047200d
// module: CEngineSubPrimitiveText  (thiscall, void*, 2 params)
//
// Standard MSVC scalar deleting destructor:
//   push esi; mov esi,ecx            ; this = ecx
//   lea ecx,[esi+0x3c]; call <dtor>  ; destruct sub-object/member at this+0x3c
//   mov ecx,esi;       call <dtor>   ; run this object's own destructor
//   test byte[esp+8],1; je skip      ; second param (flags) & 1 ?
//   push esi; call operator delete; pop ecx
// skip:
//   mov eax,esi; pop esi; ret 4      ; return this
//
// The +0x3c member destructor is __fastcall (ecx = this+0x3c); modeled as a
// free __fastcall(objptr). The main destructor takes ecx = this.

struct CEngineSubPrimitiveText;
struct CEngineSubPrimitiveText_Member;

// sub-object destructor at +0x3c (thiscall)
extern void FABLE_FASTCALL CEngineSubPrimitiveText_Member_dtor(
        CEngineSubPrimitiveText_Member* thisptr);

// this object's own (non-deleting) destructor (thiscall)
extern void FABLE_FASTCALL CEngineSubPrimitiveText_dtor(
        CEngineSubPrimitiveText* thisptr);

extern void FABLE_CDECL fable_operator_delete(void* p);

void* FABLE_FASTCALL CEngineSubPrimitiveText_scalar_deleting_dtor(
        CEngineSubPrimitiveText* thisptr,
        int /*edx*/,
        unsigned int flags)
{
    CEngineSubPrimitiveText_Member_dtor(
        (CEngineSubPrimitiveText_Member*)((fable_u8*)thisptr + 0x3c));
    CEngineSubPrimitiveText_dtor(thisptr);
    if (flags & 1)
        fable_operator_delete(thisptr);
    return thisptr;
}