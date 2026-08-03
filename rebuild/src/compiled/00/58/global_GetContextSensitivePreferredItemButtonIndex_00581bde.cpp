#pragma optimize("s",on)
struct Entry { int key; int pad; int value; };
extern Entry* g_itemButtonTable_first;
extern Entry* g_itemButtonTable_last;

int __fastcall GetContextSensitivePreferredItemButtonIndex(int key)
{
    Entry* p = g_itemButtonTable_first;
    Entry* e = g_itemButtonTable_last;
    for (; p != e; ++p)
    {
        if (p->key == key)
            return p->value;
    }
    return -1;
}