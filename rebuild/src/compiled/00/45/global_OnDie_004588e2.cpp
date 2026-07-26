#pragma optimize("s",on)
#include "rebuild_abi.h"

struct CTCInventoryItem
{
    char pad0[0x54];
    void* field_54; // +0x54
};

// __cdecl free-style cleanup on the +0x54 pointer (one stack arg, caller-cleaned via pop ecx)
extern "C" void __cdecl free_field_54(void* p);

// base-class OnDie, tail-called with this in ecx (__fastcall modeled as __fastcall)
extern void __fastcall base_OnDie(CTCInventoryItem* thisptr);

// CTCInventoryItem::OnDie  (retail 0x004588e2)
void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr)
{
    void* p = thisptr->field_54;
    if (p != 0)
    {
        free_field_54(p);
    }
    base_OnDie(thisptr);
}