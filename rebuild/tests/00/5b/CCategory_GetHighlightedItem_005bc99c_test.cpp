#include <cstdio>

struct CCategoryItem { long value; long pad1; long pad2; long pad3; long pad4; };
struct CCategory {
    char pad0[8];
    CCategoryItem* items; /* +0x8 */
    char pad2[0x20 - 0xC];
    long highlighted; /* +0x20 */
};
long __fastcall CCategory_GetHighlightedItem(const CCategory* self)
{
    long idx = self->highlighted;
    if (idx >= 0)
        return self->items[idx].value;
    return 0;
}

int main()
{
    CCategoryItem arr[3];
    arr[0].value = 100;
    arr[1].value = 200;
    arr[2].value = 300;

    CCategory cat;
    cat.items = arr;

    cat.highlighted = 1;
    if (CCategory_GetHighlightedItem(&cat) != 200) { std::printf("FAIL idx1\n"); return 1; }

    cat.highlighted = 2;
    if (CCategory_GetHighlightedItem(&cat) != 300) { std::printf("FAIL idx2\n"); return 1; }

    cat.highlighted = -1;
    if (CCategory_GetHighlightedItem(&cat) != 0) { std::printf("FAIL neg\n"); return 1; }

    std::printf("CCategory_005bc99c_TEST PASS\n");
    return 0;
}