#pragma optimize("s",on)
#include "rebuild_abi.h"

// Retail 0x00458c66 CTCAssassinRush::FinishMove (__fastcall, void, 33 bytes)
// this+0x68: sub-object; a member fn is called on it (lea ecx,[esi+0x68]; call rel32)
// this+0x50: a heap pointer; if non-null it is freed (push eax; call free; pop ecx)
// tail-jmp to base-class FinishMove with ecx=this.

struct CTCAssassinRush {
    char _pad[0x68];
    // at 0x68: sub-object (opaque)
};

// member fn invoked on this+0x68 (relocation-masked target)
extern "C" void __fastcall AssassinRush_Sub0x68_Method(void* thisptr);

// free-like callee (single stack arg, cdecl, caller pops via 'pop ecx')
extern "C" void __cdecl AssassinRush_Free(void* p);

// base-class FinishMove (tail-called with ecx=this)
extern "C" void __fastcall AssassinRush_BaseFinishMove(void* thisptr);

void __fastcall CTCAssassinRush_FinishMove(CTCAssassinRush* thisptr)
{
    unsigned char* base = (unsigned char*)thisptr;

    AssassinRush_Sub0x68_Method(base + 0x68);

    void* p = *(void**)(base + 0x50);
    if (p) {
        AssassinRush_Free(p);
    }

    AssassinRush_BaseFinishMove(thisptr);
}