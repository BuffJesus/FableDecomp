#pragma optimize("s",on)
// CTCInventoryClothing::AddClothing @ 0x005b4753
struct Item;

// call 0x5b43a2 : two stack args, __stdcall (no caller cleanup)
extern void __stdcall Sub_5b43a2(long item, long member4);

struct CTCInventoryClothing {
    void* vtbl;      // +0
    long  member4;   // +4
    // 0x5bdfb1 modeled as a thiscall member (self in ecx, one stack arg)
    void Sub_5bdfb1(long item);
    void AddClothing(long item);
};

void CTCInventoryClothing::AddClothing(long item)
{
    Sub_5b43a2(item, this->member4);
    this->Sub_5bdfb1(item);
}