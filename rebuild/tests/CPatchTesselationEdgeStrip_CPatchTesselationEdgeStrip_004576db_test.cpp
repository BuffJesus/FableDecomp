#include <cstdio>

struct CPatchTesselationEdgeStrip {
    unsigned char _pad0[0x78];
    void* p78;
    unsigned char _pad1[0x8c-0x7c];
    void* p8c;
    unsigned char _pad2[0x98-0x90];
    void* p98;
};

static int g_freed = 0;
static void* g_last[3];
extern "C" void __cdecl EdgeStrip_freefn(void* p) { if (g_freed < 3) g_last[g_freed] = p; g_freed++; }
static int g_base = 0;
static CPatchTesselationEdgeStrip* g_baseSelf = 0;
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* self) { g_base++; g_baseSelf = self; }

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p98) EdgeStrip_freefn(self->p98);
    if (self->p8c) EdgeStrip_freefn(self->p8c);
    if (self->p78) EdgeStrip_freefn(self->p78);
    CPatchTesselationEdgeStrip_base_dtor(self);
}

int main()
{
    CPatchTesselationEdgeStrip o;
    o.p98 = (void*)0x1111;
    o.p8c = (void*)0x2222;
    o.p78 = (void*)0x3333;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed != 3) { std::printf("FAIL freed=%d\n", g_freed); return 1; }
    if (g_last[0] != (void*)0x1111 || g_last[1] != (void*)0x2222 || g_last[2] != (void*)0x3333) { std::printf("FAIL order\n"); return 1; }
    if (g_base != 1 || g_baseSelf != &o) { std::printf("FAIL base\n"); return 1; }

    // null members: nothing freed, base still called
    g_freed = 0; g_base = 0;
    CPatchTesselationEdgeStrip o2;
    o2.p98 = 0; o2.p8c = 0; o2.p78 = 0;
    CPatchTesselationEdgeStrip_dtor(&o2);
    if (g_freed != 0 || g_base != 1) { std::printf("FAIL null path\n"); return 1; }

    std::printf("CPatchTesselationEdgeStrip_004576db_TEST PASS\n");
    return 0;
}