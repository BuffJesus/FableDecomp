#include <cstdio>

struct CVillagerInteractionsDef { char pad[0x48]; };

unsigned long __fastcall CVillagerInteractionsDef_GetSizeofClass(const CVillagerInteractionsDef* self)
{
    (void)self;
    return 0x48;
}

int main()
{
    CVillagerInteractionsDef obj;
    unsigned long r = CVillagerInteractionsDef_GetSizeofClass(&obj);
    if (r != 0x48) { std::printf("FAIL: got %lu\n", r); return 1; }
    std::printf("CVillagerInteractionsDef_00456f23_TEST PASS\n");
    return 0;
}