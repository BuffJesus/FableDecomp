#include <cstdio>

struct SubA { int a; };
struct SubB { int b; };

static int g_log[8];
static int g_n;

struct CPersistContext {
    void PersistA(SubA* obj) { g_log[g_n++] = 0xA0000000 | (obj->a); }
    void PersistB(SubB* obj) { g_log[g_n++] = 0xB0000000 | (obj->b); }
};

struct CEngineLocalDetailLayerDef {
    char pad[0x3c];
    SubA m3c;
    char pad2[0x48 - 0x3c - (int)sizeof(SubA)];
    SubB m48;
    void Transfer(CPersistContext* ctx);
};

void CEngineLocalDetailLayerDef::Transfer(CPersistContext* ctx)
{
    ctx->PersistA(&this->m3c);
    ctx->PersistB(&this->m48);
}

int main()
{
    CEngineLocalDetailLayerDef d;
    d.m3c.a = 0x11;
    d.m48.b = 0x22;
    CPersistContext ctx;
    g_n = 0;
    d.Transfer(&ctx);
    // Expect PersistA then PersistB, in order, with the right subobjects.
    if (g_n == 2 && g_log[0] == (int)0xA0000011 && g_log[1] == (int)0xB0000022) {
        printf("PARITY_OK_477cb1\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}