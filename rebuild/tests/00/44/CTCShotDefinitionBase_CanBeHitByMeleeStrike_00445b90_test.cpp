#include <cstdio>
struct CTCShotSub { char pad[0x49]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x7a0]; CTCShotSub* sub; };
bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self) {
    return self->sub->flag;
}
int main() {
    CTCShotSub s; s.flag = true;
    CTCShotDefinitionBase d; d.sub = &s;
    if (CTCShotDefinitionBase_CanBeHitByMeleeStrike(&d) != true) { std::printf("FAIL true case\n"); return 1; }
    s.flag = false;
    if (CTCShotDefinitionBase_CanBeHitByMeleeStrike(&d) != false) { std::printf("FAIL false case\n"); return 1; }
    std::printf("CTCShotDefinitionBase_00445b90_TEST PASS\n");
    return 0;
}