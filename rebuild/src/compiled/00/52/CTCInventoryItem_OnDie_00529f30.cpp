// CTCInventoryItem::OnDie  @ 0x00529f30
// void __fastcall (CTCInventoryItem*)

void __cdecl FreeThing(void* p);   // 0x00bfea14 (cdecl deleter)

struct CTCInventoryItemBase
{
    void* m_vptr;                  // +0
    void OnDie();                  // 0x0099eae0 (base OnDie, thiscall)
};

struct CTCInventoryItem : public CTCInventoryItemBase
{
    void* m_pField4;               // +4
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->m_pField4 != 0)
        FreeThing(this->m_pField4);
    this->CTCInventoryItemBase::OnDie();
}