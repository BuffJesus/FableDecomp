#include <cstdio>

static int g_freed = 0;
static void* g_last[4];
static int g_base = 0;

extern "C" void __cdecl eng_free(void* p) { g_last[g_freed++ & 3] = p; }
void __fastcall CBase_dtor(void* self) { g_base++; (void)self; }

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x48];
    void* p48;   // +0x48
    unsigned char _pad_0x4c[0x8];
    void* p54;   // +0x54
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54)
        eng_free(self->p54);
    if (self->p48)
        eng_free(self->p48);
    CBase_dtor(self);
}

int main() {
    CPatchTesselationEdgeStrip o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((unsigned char*)&o)[i] = 0;
    int a, b;
    o.p48 = &a;
    o.p54 = &b;
    g_freed = 0; g_base = 0;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed != 2) { std::printf("FAIL freed=%d\n", g_freed); return 1; }
    if (g_last[0] != &b) { std::printf("FAIL order54\n"); return 1; }
    if (g_last[1] != &a) { std::printf("FAIL order48\n"); return 1; }
    if (g_base != 1) { std::printf("FAIL base\n"); return 1; }

    // null case
    o.p48 = 0; o.p54 = 0;
    g_freed = 0; g_base = 0;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed != 0) { std::printf("FAIL nullfree\n"); return 1; }
    if (g_base != 1) { std::printf("FAIL nullbase\n"); return 1; }

    std::printf("CPatchTesselationEdgeStrip_00457ed6_TEST PASS\n");
    return 0;
}