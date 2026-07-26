#include <cstdio>

struct CMember {
    void* v;
    void __fastcall dtor();
};
static int g_calls = 0;
void __fastcall CMember::dtor() { g_calls++; }

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x3c];
    void* p3c;
    unsigned char _pad_0x40[0x18];
    void* p58;
    unsigned char _pad_0x5c[0x8];
    CMember m64;
    CMember m68;
};

static int g_freed = 0;
void __cdecl engine_free(void* p) { g_freed++; }
static int g_base = 0;
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* s) { g_base++; }

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    self->m68.dtor();
    self->m64.dtor();
    if (self->p58) engine_free(self->p58);
    if (self->p3c) engine_free(self->p3c);
    CPatchTesselationEdgeStrip_base_dtor(self);
}

int main()
{
    CPatchTesselationEdgeStrip o;
    o.p3c = (void*)1;
    o.p58 = (void*)1;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_calls == 2 && g_freed == 2 && g_base == 1) {
        std::printf("CPatchTesselationEdgeStrip_00457e03_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d freed=%d base=%d\n", g_calls, g_freed, g_base);
    return 1;
}