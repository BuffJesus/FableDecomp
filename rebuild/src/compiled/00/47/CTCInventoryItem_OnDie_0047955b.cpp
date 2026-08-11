#pragma optimize("s",on)
struct CTCInventoryItem
{
    char pad[0x3c];
    void* field3c;
};

extern "C" void __cdecl InvItem_Free(void* p);        // 0xbfea14
extern "C" void __fastcall InvItem_BaseOnDie(CTCInventoryItem* self); // 0x430300

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->field3c;
    if (p)
        InvItem_Free(p);
    InvItem_BaseOnDie(self);
}