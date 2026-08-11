#pragma optimize("s",on)
// CTCInventoryItem::OnDie @ 004de60a
struct CTCInventoryItem {
    char pad[0x10];
    void* f10;   // +0x10
};

extern "C" void __cdecl free_bfea14(void* p);         // 0xbfea14
extern void __fastcall base_686830(CTCInventoryItem* self); // 0x686830

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->f10)
        free_bfea14(self->f10);
    base_686830(self);
}