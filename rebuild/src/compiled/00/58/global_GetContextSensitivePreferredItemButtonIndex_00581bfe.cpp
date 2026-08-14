// Byte-exact reconstruction of 0x00581bfe
// GetContextSensitivePreferredItemButtonIndex(long action) -> long
// Linear search over an array of 12-byte {long key; long value; long pad}
// entries; returns value for matching key, else -1.

struct ItemButtonEntry {
    long key;
    long value;
    long pad;
};

extern ItemButtonEntry* g_itemButtonBegin;
extern ItemButtonEntry* g_itemButtonEnd;

#pragma optimize("s",on)
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