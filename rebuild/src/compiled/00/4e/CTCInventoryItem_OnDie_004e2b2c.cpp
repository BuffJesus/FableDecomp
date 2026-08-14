#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e2b2c
struct CTCInventoryItem {
    void* vtbl;      // +0x00
    char pad[0x24];  // +0x04 .. +0x27
    void* p28;       // +0x28
};

// cdecl free (push arg; call; pop ecx cleanup)
extern "C" void __cdecl free_thing(void* p);

// base class OnDie (this-call) forwarded to via tail jmp
void __fastcall Base_OnDie(void* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->p28 != 0)
        free_thing(self->p28);
    self->vtbl = (void*)0x1230ba0;
    Base_OnDie(self);
}