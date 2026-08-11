// CTCInventoryItem::OnDie  @ 0x00529cc0  (__fastcall, void)
// push esi; esi=this; eax=[this+0x10]; if(eax){ free_helper(eax); } Base::OnDie(this) tail-jmp

// cdecl free-style helper at 0x00bfea14
extern "C" void __cdecl FreeHelper(void* p);

struct CInventoryItemBase
{
    void OnDie();          // non-virtual, at 0x0099a2e0
};

struct CTCInventoryItem : public CInventoryItemBase
{
    char pad[0x10];        // 0x00 .. 0x0f
    void* m_ptr;           // 0x10
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->m_ptr;
    if (p != 0)
        FreeHelper(p);
    CInventoryItemBase::OnDie();
}