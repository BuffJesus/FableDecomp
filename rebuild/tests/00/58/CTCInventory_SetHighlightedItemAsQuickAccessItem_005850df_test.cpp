#include <cstdio>

struct CItem { int id; };
struct CTCInventory {
    CItem* highlighted;
    int lastItem;
    long lastArg;
    int lastFlag;
    int calls;
    CItem* GetHighlightedItem() { return highlighted; }
    void AddQuickAccess(CItem* item, long arg, int f) {
        lastItem = item ? item->id : -1;
        lastArg = arg;
        lastFlag = f;
        calls++;
    }
    void SetHighlightedItemAsQuickAccessItem(long arg) {
        CItem* item = GetHighlightedItem();
        if (item) {
            AddQuickAccess(item, arg, 0);
        }
    }
};

static CItem items[2];
static CTCInventory invs[2];

int main() {
    items[0].id = 77;
    CTCInventory* p = invs;
    p->highlighted = items;
    p->calls = 0; p->lastItem = 0; p->lastArg = 0; p->lastFlag = 9;
    p->SetHighlightedItemAsQuickAccessItem(42);
    bool ok = (p->calls == 1 && p->lastItem == 77 && p->lastArg == 42 && p->lastFlag == 0);

    CTCInventory* q = invs + 1;
    q->highlighted = 0;
    q->calls = 0;
    q->SetHighlightedItemAsQuickAccessItem(5);
    ok = ok && (q->calls == 0);

    printf(ok ? "BEHAVIOR_PASS_5850df\n" : "BEHAVIOR_FAIL\n");
    return 0;
}