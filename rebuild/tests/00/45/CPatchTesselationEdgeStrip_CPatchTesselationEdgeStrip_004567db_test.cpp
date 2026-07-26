#include <cstdio>
#include <cstdlib>

struct CPatchTesselationEdgeStrip {
    char pad0[0x3c];
    void* p3c;
    char pad1[0x48 - 0x3c - 4];
    void* p48;
};

static int g_freed = 0;
static int g_base = 0;

extern "C" void __fastcall BaseDtor(CPatchTesselationEdgeStrip* self) { g_base++; }

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p48)
        operator delete(self->p48);
    if (self->p3c)
        operator delete(self->p3c);
    BaseDtor(self);
}

int main() {
    CPatchTesselationEdgeStrip obj;
    for (int i = 0; i < (int)sizeof(obj); i++) ((char*)&obj)[i] = 0;
    obj.p48 = operator new(16);
    obj.p3c = operator new(16);
    CPatchTesselationEdgeStrip_dtor(&obj);
    if (g_base != 1) { std::printf("FAIL base=%d\n", g_base); return 1; }

    CPatchTesselationEdgeStrip obj2;
    for (int i = 0; i < (int)sizeof(obj2); i++) ((char*)&obj2)[i] = 0;
    CPatchTesselationEdgeStrip_dtor(&obj2);
    if (g_base != 2) { std::printf("FAIL base2=%d\n", g_base); return 1; }

    std::printf("CPatchTesselationEdgeStrip_004567db_TEST PASS\n");
    return 0;
}