// CTCInventoryItem::OnDie  @ 0x00a9d3a0  __fastcall void(CTCInventoryItem*)
// push esi; mov esi,ecx; eax=[esi+0x2c]; if(eax){push eax; call cdecl_free; add esp,4;}
// ecx=esi; jmp Base::OnDie  (tail call)

extern "C" void __cdecl FreeThing(void* p);   // cdecl helper @ 0xbfea14

struct BaseItem {
    void OnDie();                              // @ 0xaa8490 (non-virtual)
};

struct CTCInventoryItem : public BaseItem {
    char pad00[0x2c];
    void* ptr2c;
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->ptr2c)
        FreeThing(this->ptr2c);
    BaseItem::OnDie();
}