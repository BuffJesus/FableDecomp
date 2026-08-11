// CTCInventoryItem::RemoveFromInventory  retail 0x005d8e20
// __fastcall void(CTCInventoryItem* this)

struct COwner
{
    // non-virtual member called at 0x5bc499, __fastcall, one int arg
    void Release(int slot);
};

struct CTCInventoryItem
{
    int    field0;      // +0x00
    int    slot;        // +0x04  (arg pushed to the call)
    int    field8;      // +0x08
    int    fieldC;      // +0x0C
    COwner* owner;      // +0x10  (this for the call; zeroed after)

    void RemoveFromInventory();
};

void CTCInventoryItem::RemoveFromInventory()
{
    if (this->owner != 0)
    {
        this->owner->Release(this->slot);
        this->owner = 0;
    }
}