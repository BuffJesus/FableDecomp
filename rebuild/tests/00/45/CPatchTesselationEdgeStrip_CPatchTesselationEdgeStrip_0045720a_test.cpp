#include <cstdio>

struct CSub { };
int g_sub = 0;
int g_del = 0;
int g_base = 0;

void __fastcall CSub_dtor(CSub* self) { (void)self; g_sub++; }
void __cdecl op_delete(void* p) { (void)p; g_del++; }
void __fastcall CBase_dtor(void* self) { (void)self; g_base++; }

struct CPatchTesselationEdgeStrip {
    char pad0[0x60];
    void* p60;
    char pad1[0x08];
    void* p6c;
    char pad2[0x08];
    void* p78;
    char pad3[0x08];
    void* p84;
    char pad4[0x08];
    CSub sub90;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CSub_dtor(&self->sub90);
    if (self->p84) op_delete(self->p84);
    if (self->p78) op_delete(self->p78);
    if (self->p6c) op_delete(self->p6c);
    if (self->p60) op_delete(self->p60);
    CBase_dtor(self);
}

int main()
{
    CPatchTesselationEdgeStrip a;
    int x0, x1;
    a.p60 = &x0; a.p6c = 0; a.p78 = &x1; a.p84 = 0;
    CPatchTesselationEdgeStrip_dtor(&a);
    if (g_sub == 1 && g_base == 1 && g_del == 2) {
        std::printf("CPatchTesselationEdgeStrip_0045720a_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL sub=%d base=%d del=%d\n", g_sub, g_base, g_del);
    return 1;
}