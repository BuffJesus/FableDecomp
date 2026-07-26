#include <cstdio>
struct Field { unsigned char b[4]; };
static int g_calls = 0;
static void* g_last = 0;
struct CPersistContext {
    void PersistA(void* p){ g_calls++; g_last = p; }
    void PersistB(void* p){ g_calls++; g_last = p; }
};
struct CContextSensitiveItemDef {
    unsigned char _pad[0x28];
    Field f0;
    Field f1;
    Field f2;
    Field f3;
    Field f4;
    void Transfer(CPersistContext& ctx);
};

void CContextSensitiveItemDef::Transfer(CPersistContext& ctx)
{
    ctx.PersistA(&f0);
    ctx.PersistB(&f1);
    ctx.PersistB(&f2);
    ctx.PersistB(&f3);
    ctx.PersistB(&f4);
}

int main(){
    CContextSensitiveItemDef obj;
    CPersistContext ctx;
    obj.Transfer(ctx);
    if (g_calls == 5 && g_last == (void*)&obj.f4) {
        std::printf("CContextSensitiveItemDef_004526b3_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}