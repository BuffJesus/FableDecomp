// CTCInventoryItem::OnDie  @ 009d3b70  __fastcall void(CTCInventoryItem*)
extern "C" void __cdecl freemem_009d3b70(void* p);

struct CTCInventoryItemBase
{
    void* vtbl;      // +0x00
    void* thing;     // +0x04
    void OnDieBase(); // -> 0x0099eae0
};

struct CTCInventoryItem : public CTCInventoryItemBase
{
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->thing;
    if (p != 0)
        freemem_009d3b70(p);
    this->OnDieBase();
}