#include "rebuild_abi.h"

// CSpecialAbilitiesPhysicalShieldDef::Copy(CDefClassBase const*)
// Retail body is a single 5-byte `jmp rel32` tail-call to a shared Copy
// implementation (the rel32 target is relocation-masked in parity).
struct CDefClassBase;
struct CSpecialAbilitiesPhysicalShieldDef;

// Shared target of the tail-call. Same signature/convention so the call
// site degenerates to a single jmp under VC7.1 tail-call optimization.
extern CSpecialAbilitiesPhysicalShieldDef* __fastcall
    CSpecialAbilitiesPhysicalShieldDef_Copy_impl(
        CSpecialAbilitiesPhysicalShieldDef* thisptr, void* /*edx*/,
        const CDefClassBase* src);

CSpecialAbilitiesPhysicalShieldDef* __fastcall
CSpecialAbilitiesPhysicalShieldDef_Copy(
    CSpecialAbilitiesPhysicalShieldDef* thisptr, void* edx,
    const CDefClassBase* src)
{
    return CSpecialAbilitiesPhysicalShieldDef_Copy_impl(thisptr, edx, src);
}