// CTCInventoryItem::OnDie  0x004e0fce  __fastcall(void)
typedef unsigned int uintptr_t32;

struct CTCInventoryItem {
    void* vtbl;      // +0x00
    char pad[0x40-4];
    void* m40;       // +0x40
};

// free-ish cleanup callee at 0xbfea14 (cdecl: caller pops -> pop ecx)
extern "C" void __cdecl sub_bfea14(void* p);

// tail-call member target at 0x9fc550 (__fastcall)
void __fastcall sub_9fc550(CTCInventoryItem* self);

#pragma optimize("s",on)
void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->m40)
        sub_bfea14(self->m40);
    self->vtbl = (void*)0x1230ba0;
    sub_9fc550(self);
}