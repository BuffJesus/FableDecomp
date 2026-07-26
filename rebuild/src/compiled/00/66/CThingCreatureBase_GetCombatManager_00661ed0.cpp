// CThingCreatureBase::GetCombatManager @ 0x00661ed0
// Retail bytes: e8 cb 5b e6 ff  8b 40 4c  c3
//   call   <helper>            ; this (ecx) passed through
//   mov    eax,[eax+0x4c]      ; load field at +0x4c of returned base
//   ret
//
// Modeled as __fastcall (ecx = this) per VC7.1 thiscall rules for a free fn.

struct CThingCreatureBase; // opaque
struct CCombatManager;      // opaque

// Relocation-masked engine helper. Returns some base pointer in eax whose
// field at +0x4c holds the CCombatManager*. It is thiscall on the object
// (ecx = this); modeled as __fastcall taking the object pointer in ecx.
// Declared __declspec(noinline) so cl emits a real 'call rel32' and does not
// fold the two-step load.
extern void * __fastcall CThingCreatureBase_GetCombatBase(CThingCreatureBase *self);

CCombatManager * __fastcall CThingCreatureBase_GetCombatManager(CThingCreatureBase *self)
{
    // call helper (this passed through), then read [eax+0x4c]
    unsigned char *base = (unsigned char *)CThingCreatureBase_GetCombatBase(self);
    return *(CCombatManager **)(base + 0x4c);
}