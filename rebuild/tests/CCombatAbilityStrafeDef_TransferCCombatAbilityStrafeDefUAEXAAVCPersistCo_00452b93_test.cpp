#include <cstdio>
struct CFieldA { int a; };
struct CFieldB { int b; };

static int g_calls = 0;
static void* g_argA = 0;
static void* g_argB = 0;
static void* g_ctxA = 0;
static void* g_ctxB = 0;

struct CPersistContext {
    void TransferA(CFieldA* p){ g_calls++; g_ctxA = this; g_argA = p; }
    void TransferB(CFieldB* p){ g_calls++; g_ctxB = this; g_argB = p; }
};

struct CCombatAbilityStrafeDef {
    unsigned char _pad28[0x28];
    CFieldB f28;
    unsigned char _pad40[0x40 - 0x28 - sizeof(CFieldB)];
    CFieldA f40;
};

void __fastcall CCombatAbilityStrafeDef_Transfer(CCombatAbilityStrafeDef* self, int, CPersistContext* ctx)
{
    ctx->TransferA(&self->f40);
    ctx->TransferB(&self->f28);
}

int main(){
    CCombatAbilityStrafeDef obj;
    CPersistContext ctx;
    CCombatAbilityStrafeDef_Transfer(&obj, 0, &ctx);
    if (g_calls != 2) { std::printf("FAIL calls=%d\n", g_calls); return 1; }
    if (g_argA != (void*)((char*)&obj + 0x40)) { std::printf("FAIL argA\n"); return 1; }
    if (g_argB != (void*)((char*)&obj + 0x28)) { std::printf("FAIL argB\n"); return 1; }
    if (g_ctxA != &ctx || g_ctxB != &ctx) { std::printf("FAIL ctx\n"); return 1; }
    std::printf("CCombatAbilityStrafeDef_00452b93_TEST PASS\n");
    return 0;
}