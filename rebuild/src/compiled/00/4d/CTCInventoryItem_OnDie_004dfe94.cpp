#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004dfe94

struct CTCInventoryItem;

// free cdecl callee @ 0xbfea14 (push arg; call; pop ecx cleanup)
extern "C" void __cdecl FreeSlot(void* p);

// base OnDie member forwarder @ 0x9fc550 (this in ecx)
void __fastcall BaseOnDie(CTCInventoryItem* self);

struct CTCInventoryItem {
    void** vtbl;      // +0x00
    char pad[0x28 - 4];
    void* field28;    // +0x28
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->field28) {
        FreeSlot(self->field28);
    }
    self->vtbl = (void**)0x1230ba0;
    BaseOnDie(self);
}