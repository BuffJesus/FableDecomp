#include <cstdio>

struct CMemberDef {
    int calls;
    const CMemberDef* last;
    void __fastcall Copy(const CMemberDef* other) { calls++; last = other; }
};

struct CBaseState {
    int calls;
    const void* last;
    void __fastcall Copy(const CBaseState* other) { calls++; last = other; }
};

struct CCombatAbilityFlourishCounterAttackDef {
    CBaseState base;      // base region (offset 0)
    unsigned char pad[0x28 - sizeof(CBaseState)];
    CMemberDef m_member;  // at +0x28
    unsigned char tail[0x18 - sizeof(CMemberDef)];
};

void __fastcall CCombatAbilityFlourishCounterAttackDef_Copy(
        CCombatAbilityFlourishCounterAttackDef* self,
        int edx_ignore,
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    self->base.Copy(&other->base);
    self->m_member.Copy(&other->m_member);
}

int main() {
    CCombatAbilityFlourishCounterAttackDef a, b;
    a.base.calls = 0; a.m_member.calls = 0;
    CCombatAbilityFlourishCounterAttackDef_Copy(&a, 0, &b);
    if (a.base.calls == 1 && a.base.last == &b.base &&
        a.m_member.calls == 1 && a.m_member.last == &b.m_member) {
        std::printf("CCombatAbilityFlourishCounterAttackDef_00453ed2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}