#include <stdio.h>
struct CategoryEntry { void* item; int a, b, c, d; };
struct CCategory {
    char pad0[0x8];
    CategoryEntry* entries;
    char pad8[0x24 - 0xC];
    int highlightIndex;
};
void* __fastcall GetHighlightedItem(CCategory* this_) {
    int idx = this_->highlightIndex;
    if (idx < 0)
        return 0;
    return this_->entries[idx].item;
}
int main() {
    CategoryEntry ents[4];
    for (int i = 0; i < 4; i++) { ents[i].item = (void*)(0x1000 + i); ents[i].a = ents[i].b = ents[i].c = ents[i].d = 0; }
    CCategory cat;
    cat.entries = ents;
    cat.highlightIndex = 2;
    void* r = GetHighlightedItem(&cat);
    if (r != (void*)0x1002) { printf("FAIL idx2 got %p\n", r); return 1; }
    cat.highlightIndex = 0;
    r = GetHighlightedItem(&cat);
    if (r != (void*)0x1000) { printf("FAIL idx0 got %p\n", r); return 1; }
    cat.highlightIndex = -1;
    r = GetHighlightedItem(&cat);
    if (r != 0) { printf("FAIL neg got %p\n", r); return 1; }
    printf("OK_0x005bc988\n");
    return 0;
}