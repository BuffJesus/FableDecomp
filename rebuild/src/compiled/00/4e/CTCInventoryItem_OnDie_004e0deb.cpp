#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e0deb
// thiscall(void) modeled as __fastcall(self) so this arrives in ecx.

typedef unsigned long DWORD;

struct CTCInventoryItem {
    DWORD* vtable;      // +0x00
    char pad[0x28 - 4]; // up to +0x28
    void* p28;          // +0x28
};

// cdecl callee at 0xbfea14 (single-arg free/delete-like), stack cleaned by caller (pop ecx)
extern "C" void __cdecl free_helper(void* p);

// base destructor / OnDie at 0x9fc550, thiscall -> model as __fastcall(self)
extern void __fastcall base_OnDie(CTCInventoryItem* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->p28;
    if (p) {
        free_helper(p);
    }
    self->vtable = (DWORD*)0x1230ba0;
    base_OnDie(self);
}