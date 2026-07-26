#pragma optimize("s",on)
struct CTCInventoryItem {
    void* vptr;
    unsigned char _pad4[0x94];
    void* ptr98; // +0x98
};

extern "C" void freemem(void* p);
extern void __fastcall Base_OnDie(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    void* p = self->ptr98;
    if (p != 0)
        freemem(p);
    Base_OnDie(self);
}