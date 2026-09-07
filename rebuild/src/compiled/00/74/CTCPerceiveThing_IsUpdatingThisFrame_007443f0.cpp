// CTCPerceiveThing::IsUpdatingThisFrame @ 0x007443f0
// bool __fastcall, modeled as __fastcall (this in ecx).
// Retail: (GetFrameCount() % 4) == this->UpdateOffset
//
// Retail disasm target:
//   push esi
//   mov  esi,ecx
//   call GetFrameCount            ; returns signed int in eax
//   and  eax,0x80000003           ; signed x % 4 ...
//   jns  +5
//   dec  eax
//   or   eax,0xfffffffc
//   inc  eax
//   sub  eax,[esi+0x38]           ; - this->UpdateOffset
//   neg  eax
//   sbb  eax,eax
//   inc  eax                      ; (result==0) ? 1 : 0
//   pop  esi
//   ret

// Engine global frame-count getter; call rel32 is relocation-masked in parity.
#include "engine/CTCPerceiveThing.h"  // retyped onto the PDB layout; byte parity re-verified
extern int __cdecl GetFrameCount(void);


// __fastcall places 'this' in ecx, byte-identical to __fastcall for a this-only accessor.
bool __fastcall CTCPerceiveThing_IsUpdatingThisFrame(CTCPerceiveThing *self)
{
    return (GetFrameCount() % 4) == self->UpdateOffset;
}