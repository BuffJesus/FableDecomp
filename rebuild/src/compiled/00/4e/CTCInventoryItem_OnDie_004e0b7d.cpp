#pragma optimize("s",on)
// Reconstruction of CTCInventoryItem::OnDie @ 0x004e0b7d
typedef unsigned long DWORD;

struct CTCInventoryItem {
    DWORD vtbl;      // +0x00
    char pad[0x30];  // +0x04 .. +0x33
    void* p34;       // +0x34
};

extern "C" void __cdecl FreeHelper(void* p);          // 0xbfea14 (cdecl, pop ecx cleanup)
void __fastcall BaseOnDie(void* self);                // 0x9fc550

#define VTBL_CTCINVENTORYITEM ((DWORD)0x1230ba0)

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->p34 != 0)
        FreeHelper(self->p34);
    self->vtbl = VTBL_CTCINVENTORYITEM;
    BaseOnDie(self);
}