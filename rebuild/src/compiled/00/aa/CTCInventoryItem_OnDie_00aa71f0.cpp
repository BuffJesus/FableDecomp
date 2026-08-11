// CTCInventoryItem::OnDie @ 0x00aa71f0
// push esi; mov esi,ecx; eax=[esi+0x40]; if(eax){push eax; call free; add esp,4}
// mov ecx,esi; jmp base::OnDie
extern "C" void __cdecl free_thing(void* p);   // cdecl free @ 0xbfea14

struct CTCInventoryItemBase {
    char pad[0x40];
    void* m_ptr;   // +0x40
    void BaseOnDie();   // __fastcall @ 0xaa75c0
};

struct CTCInventoryItem : public CTCInventoryItemBase {
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->m_ptr)
        free_thing(this->m_ptr);
    this->BaseOnDie();
}