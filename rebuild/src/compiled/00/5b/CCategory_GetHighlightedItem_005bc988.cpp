struct CategoryEntry { void* item; int a, b, c, d; };
struct CCategory {
    char pad0[0x8];
    CategoryEntry* entries;
    char pad8[0x24 - 0xC];
    int highlightIndex;
};
void* __fastcall GetHighlightedItem(CCategory* this_) {
    int idx = this_->highlightIndex;
    if (idx >= 0)
        return this_->entries[idx].item;
    return 0;
}