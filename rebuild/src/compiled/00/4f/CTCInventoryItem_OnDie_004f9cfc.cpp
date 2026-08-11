#pragma optimize("s",on)
// CTCInventoryItem::OnDie @ 004f9cfc
struct CTCInventoryItem {
    char pad[0x3c];
    void* f3c;
};

extern "C" void __cdecl free_bfea14(void* p);
void __fastcall base_430300(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->f3c)
        free_bfea14(self->f3c);
    base_430300(self);
}