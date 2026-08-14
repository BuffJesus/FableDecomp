#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e3e89
// this-call modeled as __fastcall free fn (this in ecx).

struct CTCInventoryItem {
    void* vtbl;      // +0x00
    char  pad[0x24]; // +0x04 .. +0x27
    void* m28;       // +0x28
};

// operator delete (cdecl) @ 0xbfea14
extern "C" void __cdecl fable_op_delete(void* p);

// base OnDie (this-call) @ 0x9fc550
void __fastcall base_OnDie(CTCInventoryItem* self);

extern const int g_CTCInventoryItem_vtbl;

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->m28)
        fable_op_delete(self->m28);
    self->vtbl = (void*)0x1230ba0;
    base_OnDie(self);
}