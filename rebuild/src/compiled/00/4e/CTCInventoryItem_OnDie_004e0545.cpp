#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e0545
// __fastcall(void) -> modeled as __fastcall(self)

struct CTCInventoryItem {
    void* vtbl;        // +0x00
    char  pad[0x24];   // +0x04 .. +0x27
    void* obj28;       // +0x28
};

// operator delete / deallocator @ 0x00bfea14 (cdecl, 1 ptr arg, caller cleanup)
extern "C" void __cdecl EngineFree(void* p);

// base-class OnDie @ 0x009fc550 (this-call, void)
extern void __fastcall BaseOnDie(CTCInventoryItem* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->obj28)
        EngineFree(self->obj28);
    self->vtbl = (void*)0x01230ba0;
    BaseOnDie(self);
}