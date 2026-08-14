#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventory::SetHighlightedItemAsQuickAccessItem @ 0x005850df
// cc=__fastcall ret=void params=CTCInventory*, long
// Modeled as a real C++ member so VC7.1 emits __fastcall (this in ecx, long on stack).

struct CItem;

struct CTCInventory {
    CItem* GetHighlightedItem();                       // @ 0x5bcae3
    void AddQuickAccess(CItem* item, long arg, int f); // @ 0x584157
    void SetHighlightedItemAsQuickAccessItem(long arg);
};

void CTCInventory::SetHighlightedItemAsQuickAccessItem(long arg)
{
    CItem* item = GetHighlightedItem();
    if (item)
    {
        AddQuickAccess(item, arg, 0);
    }
}