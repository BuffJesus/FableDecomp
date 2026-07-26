#include <cstdio>

struct CCombatAbilityData {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
    int f14;
};

CCombatAbilityData* __fastcall CCombatAbilityData_operator_assign(CCombatAbilityData* self, int edx_dummy, const CCombatAbilityData* rhs) {
    self->f4  = rhs->f4;
    self->f8  = rhs->f8;
    self->fc  = rhs->fc;
    self->f10 = rhs->f10;
    self->f14 = rhs->f14;
    return self;
}

int main() {
    CCombatAbilityData a, b;
    a.f0 = 111; a.f4 = 1; a.f8 = 2; a.fc = 3; a.f10 = 4; a.f14 = 5;
    b.f0 = 999; b.f4 = 0; b.f8 = 0; b.fc = 0; b.f10 = 0; b.f14 = 0;
    CCombatAbilityData* r = CCombatAbilityData_operator_assign(&b, 0, &a);
    if (r != &b) { std::printf("FAIL: return\n"); return 1; }
    if (b.f4 != 1 || b.f8 != 2 || b.fc != 3 || b.f10 != 4 || b.f14 != 5) { std::printf("FAIL: copy\n"); return 1; }
    if (b.f0 != 999) { std::printf("FAIL: f0 clobbered\n"); return 1; }
    std::printf("CCombatAbilityData_00453f18_TEST PASS\n");
    return 0;
}