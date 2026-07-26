#pragma optimize("s",on)
#include "rebuild_abi.h"

// CTCInventoryItem::OnDie @ 0x0045855a
// push esi; mov esi,ecx; mov eax,[esi+0x3c]; test eax,eax; je skip;
//   push eax; call <free>; pop ecx; skip: mov ecx,esi; pop esi; jmp <base::OnDie>

struct CTCInventoryItem
{
    char pad[0x3c];
    void* m_ptr;   // +0x3c
};

// cdecl free-style callee (relocation-masked target)
extern "C" void __cdecl engine_free_0045855a(void* p);

// base-class OnDie tail-called with __fastcall (this in ecx) -> model as __fastcall(this)
extern void __fastcall base_OnDie_0045855a(CTCInventoryItem* thisptr);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        engine_free_0045855a(p);
    }
    // tail call to base class OnDie with same this
    base_OnDie_0045855a(thisptr);
}