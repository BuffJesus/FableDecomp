#pragma optimize("s",on)
// CTCInventoryItem::OnDie @ 004583fa
struct CTCInventoryItem {
    char pad[0x44];
    void* f44;
};

extern "C" void my_free(void* p);                        // cdecl free @ 0xbfea14
void __fastcall base_die_forward(CTCInventoryItem* self); // tail base OnDie @ 0x430300

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->f44)
        my_free(self->f44);
    base_die_forward(self);
}