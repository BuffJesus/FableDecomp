#pragma optimize("s",on)
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x8];
    void* ptr8;
    unsigned char _pad_0xc[0x4];
    void* ptr10;
    unsigned char _pad_0x14[0xc];
    char sub20;
};
extern "C" void __cdecl eng_free(void* p);
void __fastcall CTCInventoryItem_base_OnDie(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr8;
    if (p)
        eng_free(p);
    CTCInventoryItem_base_OnDie(self);
}