#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e3e0b
// __fastcall(void)  -> modeled as __fastcall free fn (this in ecx)

struct CTCInventoryItem {
    void* vtable;      // +0x00
    char  pad[0x24];   // +0x04 .. +0x27
    void* member28;    // +0x28
};

// cdecl callee cleaned up by caller pop (single arg): operator delete / free
extern "C" void __cdecl FreeThing(void* p);          // 0xbfea14

// base OnDie / dtor, tail-jumped (this in ecx)
void __fastcall BaseOnDie(CTCInventoryItem* self);   // 0x9fc550

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->member28)
        FreeThing(self->member28);
    self->vtable = (void*)0x1230ba0;
    BaseOnDie(self);
}