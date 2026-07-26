#include <cstdio>
struct CBaseDef {
    unsigned char _b[0x28];
    void Copy(CBaseDef* other);
};
struct CMemberDef {
    unsigned char _m[0x20];
    void Copy(CMemberDef* other);
};
struct CCombatAbilityGetHitCounterAttackDef : public CBaseDef {
    CMemberDef m_member;
    unsigned char _tail[0x48 - 0x28 - 0x20];
    void Copy(CCombatAbilityGetHitCounterAttackDef* other);
};

static int g_base_calls = 0;
static int g_member_calls = 0;
static void* g_base_self = 0; static void* g_base_other = 0;
static void* g_member_self = 0; static void* g_member_other = 0;

void CBaseDef::Copy(CBaseDef* other) { g_base_calls++; g_base_self = this; g_base_other = other; }
void CMemberDef::Copy(CMemberDef* other) { g_member_calls++; g_member_self = this; g_member_other = other; }

void CCombatAbilityGetHitCounterAttackDef::Copy(CCombatAbilityGetHitCounterAttackDef* other)
{
    CBaseDef::Copy((CBaseDef*)other);
    this->m_member.Copy(&other->m_member);
}

int main() {
    CCombatAbilityGetHitCounterAttackDef a, b;
    a.Copy(&b);
    if (g_base_calls != 1) { std::printf("FAIL base_calls=%d\n", g_base_calls); return 1; }
    if (g_member_calls != 1) { std::printf("FAIL member_calls=%d\n", g_member_calls); return 1; }
    if (g_base_self != (void*)(CBaseDef*)&a) { std::printf("FAIL base_self\n"); return 1; }
    if (g_base_other != (void*)(CBaseDef*)&b) { std::printf("FAIL base_other\n"); return 1; }
    if (g_member_self != (void*)((unsigned char*)&a + 0x28)) { std::printf("FAIL member_self\n"); return 1; }
    if (g_member_other != (void*)((unsigned char*)&b + 0x28)) { std::printf("FAIL member_other\n"); return 1; }
    std::printf("CCombatAbilityGetHitCounterAttackDef_00453f3f_TEST PASS\n");
    return 0;
}