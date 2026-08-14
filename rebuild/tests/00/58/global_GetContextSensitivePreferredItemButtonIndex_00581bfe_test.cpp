#include <cstdio>

struct ItemButtonEntry {
    long key;
    long value;
    long pad;
};

static ItemButtonEntry g_data[] = {
    { 10, 100, 0 },
    { 20, 200, 0 },
    { 30, 300, 0 },
};

ItemButtonEntry* g_itemButtonBegin = g_data;
ItemButtonEntry* g_itemButtonEnd = g_data + 3;

long __fastcall GetContextSensitivePreferredItemButtonIndex(long action)
{
    ItemButtonEntry* it = g_itemButtonBegin;
    ItemButtonEntry* end = g_itemButtonEnd;
    for (; it != end; ++it) {
        if (it->key == action)
            return it->value;
    }
    return -1;
}

int main()
{
    int ok = 1;
    if (GetContextSensitivePreferredItemButtonIndex(10) != 100) ok = 0;
    if (GetContextSensitivePreferredItemButtonIndex(20) != 200) ok = 0;
    if (GetContextSensitivePreferredItemButtonIndex(30) != 300) ok = 0;
    if (GetContextSensitivePreferredItemButtonIndex(99) != -1) ok = 0;
    if (GetContextSensitivePreferredItemButtonIndex(15) != -1) ok = 0;
    if (ok) printf("BEHAVIOR_OK\n");
    else printf("BEHAVIOR_FAIL\n");
    return 0;
}