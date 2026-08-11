// CTCInventoryItem::OnDie  @ 0x00aa49b0  __fastcall void(CTCInventoryItem*)
// Frees a heap pointer held at +0x10 (operator delete), then tail-calls base OnDie.

void __cdecl free_target(void* p);        // 0xbfea14  operator delete(void*)

struct CTCInventoryItem
{
    char pad0[0x10];
    void* m_ptr;          // +0x10
    void BaseOnDie();     // 0xaa75c0  __fastcall base method
    void OnDie_impl();
};

void CTCInventoryItem::OnDie_impl()
{
    void* p = this->m_ptr;
    if (p)
        free_target(p);
    this->BaseOnDie();
}