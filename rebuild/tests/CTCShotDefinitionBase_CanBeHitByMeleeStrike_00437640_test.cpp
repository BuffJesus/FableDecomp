#include <cstdio>

struct SShotSub { char pad[0x18]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x218]; SShotSub* sub; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->sub->flag;
}

int main()
{
    SShotSub s1; s1.flag = true;
    SShotSub s0; s0.flag = false;
    CTCShotDefinitionBase a; a.sub = &s1;
    CTCShotDefinitionBase b; b.sub = &s0;
    if (CTCShotDefinitionBase_CanBeHitByMeleeStrike(&a) != true) { std::printf("FAIL true\n"); return 1; }
    if (CTCShotDefinitionBase_CanBeHitByMeleeStrike(&b) != false) { std::printf("FAIL false\n"); return 1; }
    std::printf("CTCShotDefinitionBase_00437640_TEST PASS\n");
    return 0;
}