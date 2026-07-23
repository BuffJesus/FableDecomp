#pragma optimize("s",on)
struct CTCInventoryItem {
    void* vptr;      // +0x0
    char _pad4[0x40];
    void* ptr44;     // +0x44
};

extern "C" void __cdecl eng_free(void* p);
extern void __fastcall CTCInventoryItem_Base_OnDie(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr44;
    if (p != 0)
        eng_free(p);
    CTCInventoryItem_Base_OnDie(self);
}