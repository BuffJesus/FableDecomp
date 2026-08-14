// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e13df
#pragma optimize("s",on)

struct CTCInventoryItem;

// cdecl free callee at 0xbfea14 (push eax; call; pop ecx cleanup)
extern "C" void __cdecl Freer(void* p);

// base class OnDie (this-call member) at 0x9fc550
void __fastcall BaseOnDie(CTCInventoryItem* self);

struct CTCInventoryItem {
    void* vtbl;      // +0x00
    char pad[0x68];  // +0x04 .. +0x6b
    void* field6c;   // +0x6c
};

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->field6c) {
        Freer(self->field6c);
    }
    self->vtbl = (void*)0x1230ba0;
    BaseOnDie(self);
}