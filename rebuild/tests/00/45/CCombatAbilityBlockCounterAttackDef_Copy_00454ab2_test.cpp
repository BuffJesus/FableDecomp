
#include <cstdio>

struct CMember {
    int val;
    void Copy(const CMember* src) { val = src->val; }
};
struct Def {
    unsigned char base[0x28];
    CMember member;
    unsigned char tail[0x48 - 0x28 - sizeof(CMember)];
};

int base_called = 0;
void BaseCopy(Def* self, const Def* src) { base_called = 1; }

void Copy(Def* self, const Def* src)
{
    BaseCopy(self, src);
    self->member.Copy(&src->member);
}

int main() {
    Def a, b;
    b.member.val = 77;
    a.member.val = 0;
    Copy(&a, &b);
    if (base_called == 1 && a.member.val == 77) {
        std::printf("CCombatAbilityBlockCounterAttackDef_00454ab2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}