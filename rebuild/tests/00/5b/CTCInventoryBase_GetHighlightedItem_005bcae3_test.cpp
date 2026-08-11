#include <stdio.h>

struct InvItem {
    long handle;      // read at +0
    char rest[0x2c - 4];
    long GetHandle();
};
long InvItem::GetHandle() { return this->handle; }

struct InvVec {
    InvItem* first;
    InvItem* last;
    bool empty() const { return first == last; }
};

struct CTCInventoryBase {
    char pad00[0x20];
    InvVec items;     // +0x20 { first@0x20, last@0x24 }
    char pad28[0x70 - 0x28];
    int   highlight;  // +0x70
    long GetHighlightedItem();
};

long CTCInventoryBase::GetHighlightedItem()
{
    if (!this->items.empty())
        return this->items.first[this->highlight].GetHandle();
    return 0;
}

int main()
{
    int ok = 1;

    // Case 1: empty inventory (first == last) -> 0
    {
        CTCInventoryBase inv;
        inv.items.first = (InvItem*)0x1000;
        inv.items.last  = (InvItem*)0x1000;
        inv.highlight = 5;
        if (inv.GetHighlightedItem() != 0) ok = 0;
    }

    // Case 2: non-empty, highlight index 2 -> element[2].handle
    {
        InvItem items[4];
        for (int i = 0; i < 4; ++i) items[i].handle = 100 + i;
        CTCInventoryBase inv;
        inv.items.first = &items[0];
        inv.items.last  = &items[4];
        inv.highlight = 2;
        if (inv.GetHighlightedItem() != 102) ok = 0;
    }

    // Case 3: stride is exactly 44 bytes (0x2c)
    if (sizeof(InvItem) != 0x2c) ok = 0;

    if (ok) printf("GETHIGHLIGHTEDITEM_PASS\n");
    else    printf("GETHIGHLIGHTEDITEM_FAIL\n");
    return ok ? 0 : 1;
}