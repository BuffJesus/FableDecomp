#include <cstdio>

struct CCombatAbilityAttackBase {
    char pad0[0x28];
    char sub28[0x18];   // +0x28
    unsigned char pad40; // +0x40
    unsigned char pad41; // +0x41
    char pad42[2];
    unsigned short field44; // +0x44
};

static void* g_args[4];
static int g_n = 0;

struct CPersistContext {
    void TByte(unsigned char* p) { g_args[g_n++] = p; }
    void TWord(unsigned short* p) { g_args[g_n++] = p; }
    void TSub(void* p) { g_args[g_n++] = p; }
};

static void Transfer(CCombatAbilityAttackBase* self, CPersistContext* ctx)
{
    ctx->TByte(&self->pad40);
    ctx->TByte(&self->pad41);
    ctx->TWord(&self->field44);
    ctx->TSub(&self->sub28);
}

int main()
{
    CCombatAbilityAttackBase o;
    CPersistContext c;
    Transfer(&o, &c);
    char* base = (char*)&o;
    if (g_n != 4) { std::printf("FAIL count\n"); return 1; }
    if ((char*)g_args[0] - base != 0x40) { std::printf("FAIL a0\n"); return 1; }
    if ((char*)g_args[1] - base != 0x41) { std::printf("FAIL a1\n"); return 1; }
    if ((char*)g_args[2] - base != 0x44) { std::printf("FAIL a2\n"); return 1; }
    if ((char*)g_args[3] - base != 0x28) { std::printf("FAIL a3\n"); return 1; }
    std::printf("CCombatAbilityAttackBase_00452b14_TEST PASS\n");
    return 0;
}