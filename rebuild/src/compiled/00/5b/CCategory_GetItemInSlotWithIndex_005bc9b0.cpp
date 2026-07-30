#pragma optimize("s",on)
struct CItem;
struct CCategory {
    char pad[8];
    struct Slot { CItem* item; int a, b, c, d; }* slots;
};
CItem* __fastcall GetItemInSlotWithIndex(CCategory* self, int edx_dummy, int index) {
    return index >= 0 ? self->slots[index].item : 0;
}