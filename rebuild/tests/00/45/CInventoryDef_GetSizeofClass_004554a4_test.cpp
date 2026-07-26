#include <cstdio>

struct CInventoryDef;

unsigned long __fastcall CInventoryDef_GetSizeofClass(CInventoryDef* self)
{
    return 0xa34;
}

int main()
{
    unsigned long r = CInventoryDef_GetSizeofClass(0);
    if (r == 0xa34) {
        std::printf("CInventoryDef_004554a4_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}