#pragma optimize("s",on)
extern void __cdecl engine_free(void* p);
extern void __fastcall CTCInventoryItem_base(void* self);

struct CTCInventoryItem {
    void* vptr;                    // +0x00
    unsigned char _pad_0x4[0x24];  // to 0x28
    void* ptr28;                   // +0x28
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr28)
        engine_free(self->ptr28);
    self->vptr = (void*)0x1230ba0;
    CTCInventoryItem_base(self);
}