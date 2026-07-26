#include <cstdio>

struct CCategory {
    char pad[0x50];
    __int64 item;   /* 0x50 */
    long count;     /* 0x58 */
};

__int64 __fastcall CCategory_GetHighlightedItem(const CCategory* self)
{
    if (self->count < 2)
        return (__int64)-131;
    return self->item;
}

int main()
{
    CCategory a;
    a.item = 0x1122334455667788LL;
    a.count = 1;
    if (CCategory_GetHighlightedItem(&a) != (__int64)-131) { std::printf("FAIL low count\n"); return 1; }
    a.count = 2;
    if (CCategory_GetHighlightedItem(&a) != 0x1122334455667788LL) { std::printf("FAIL item\n"); return 1; }
    a.count = 5;
    if (CCategory_GetHighlightedItem(&a) != 0x1122334455667788LL) { std::printf("FAIL item2\n"); return 1; }
    std::printf("CCategory_00c8b9b0_TEST PASS\n");
    return 0;
}