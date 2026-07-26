#include <cstdio>

struct CDragonActionHoverDef { };

unsigned long __fastcall CDragonActionHoverDef_GetSizeofClass(const CDragonActionHoverDef* self)
{
    return 0x2c;
}

int main()
{
    CDragonActionHoverDef obj;
    unsigned long r = CDragonActionHoverDef_GetSizeofClass(&obj);
    if (r == 0x2c) {
        std::printf("CDragonActionHoverDef_0044c5fb_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}