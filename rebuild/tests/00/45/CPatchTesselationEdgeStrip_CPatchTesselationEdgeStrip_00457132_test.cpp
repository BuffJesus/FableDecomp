#include <cstdio>

static int g_freed = 0;
static void* g_last = 0;
extern "C" void eng_free(void* p) { g_freed++; g_last = p; }

struct CPatchTesselationEdgeStrip {
    char pad00[0x40];
    void* p40;
    char pad44[0x4c-0x44];
    void* p4c;
    char pad50[0x58-0x50];
    void* p58;
    char pad5c[0x90-0x5c];
    void* p90;
};

static int g_baseCalled = 0;
static void Base_dtor(void* self) { g_baseCalled++; }

void CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p90) eng_free(self->p90);
    if (self->p58) eng_free(self->p58);
    if (self->p4c) eng_free(self->p4c);
    if (self->p40) eng_free(self->p40);
    Base_dtor(self);
}

int main() {
    CPatchTesselationEdgeStrip o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;
    o.p40 = (void*)0x40;
    o.p4c = (void*)0x4c;
    o.p58 = 0;
    o.p90 = (void*)0x90;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed == 3 && g_baseCalled == 1) {
        std::printf("CPatchTesselationEdgeStrip_00457132_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL freed=%d base=%d\n", g_freed, g_baseCalled);
    return 1;
}