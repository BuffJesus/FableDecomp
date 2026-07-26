// CThingCreatureBase::GetCombatManager @ 0x00661ec0  (retail: e8eb5be6ff 8b404c c3)
//
// Retail disasm:
//   call   <helper>          ; rel32, relocation-masked -> returns object ptr in eax
//   mov    eax,[eax+0x4c]    ; load CombatManager* from returned object +0x4c
//   ret
//
// The accessor forwards through a helper (a tail-of-this-only call whose result
// lands in eax), then dereferences +0x4c on that result. Modeled as __fastcall so
// 'this' arrives in ecx exactly as thiscall; the helper is declared extern so the
// call/rel32 is relocation-masked.
//
// VC7.1 (MSVC 2003) constraints honored: no C++11, plain scalars, 0 not nullptr.

struct CCombatManager;

// Helper that arrives at 'this' (ecx) and returns some engine object pointer in eax.
// Declared extern; the call target is relocation-masked in parity. Using __fastcall
// keeps 'this' in ecx for the forwarded call so the emitted 'call' + eax-return
// dereference matches byte-for-byte.
struct CThingCreatureHelperResult
{
    char _pad[0x4c];
    CCombatManager *pCombatManager; // +0x4c
};

extern CThingCreatureHelperResult * __fastcall CThingCreatureBase_GetCombatManagerHelper(void *pThis);

struct CThingCreatureBase
{
    // opaque; layout irrelevant to this accessor (result comes from helper call)
    char _opaque[4];
};

CCombatManager * __fastcall CThingCreatureBase__GetCombatManager(CThingCreatureBase *pThis)
{
    // call helper (result in eax), then load [eax+0x4c]
    CThingCreatureHelperResult *pObj = CThingCreatureBase_GetCombatManagerHelper(pThis);
    return pObj->pCombatManager;
}