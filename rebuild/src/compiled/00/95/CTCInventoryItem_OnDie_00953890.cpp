// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x00953890
// __fastcall void OnDie(CTCInventoryItem* self)

struct CTCInventoryItem {
    char pad[0x120];
    void* m120;          // +0x120
};

// cdecl helper freeing the member (call 0xbfea14, add esp,4 -> cdecl)
extern "C" void __cdecl freeHelper(void* p);

// base class OnDie, tail-called (mov ecx,esi; jmp 0x62e900) -> __fastcall
struct CBase { };
void __fastcall Base_OnDie(void* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->m120;
    if (p != 0)
        freeHelper(p);
    Base_OnDie(self);
}