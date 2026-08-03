#include <cstdio>
struct Field { unsigned char b[0xc]; };
static int g_calls = 0;
static void* g_last = 0;
struct CPersistContext {
    void PersistA(void* p){ g_calls++; g_last = p; }
    void PersistB(void* p){ g_calls++; g_last = p; }
    void PersistC(void* p){ g_calls++; g_last = p; }
    void PersistD(void* p){ g_calls++; g_last = p; }
};
struct CSpecialAbilitiesDrainLifeDef {
    unsigned char _pad[0x3c];
    Field f0;
    Field f1;
    Field f2;
    Field f3;
    Field f4;
    void Transfer(CPersistContext& ctx);
};

void CSpecialAbilitiesDrainLifeDef::Transfer(CPersistContext& ctx)
{
    ctx.PersistA(&f0);
    ctx.PersistB(&f1);
    ctx.PersistA(&f2);
    ctx.PersistC(&f3);
    ctx.PersistD(&f4);
}

int main(){
    CSpecialAbilitiesDrainLifeDef obj;
    CPersistContext ctx;
    obj.Transfer(ctx);
    if (g_calls == 5 && g_last == (void*)&obj.f4) {
        std::printf("CSPECIALABILITIESDRAINLIFEDEF_TRANSFER_004724C2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}