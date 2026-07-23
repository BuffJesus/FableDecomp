#include <cstdio>

struct CMember { void dtor(); };
static int g_member_dtor = 0;
void __fastcall CMember_dtor(CMember* self) { (void)self; g_member_dtor++; }

struct CBase { void dtor(); };
static int g_base_dtor = 0;
void __fastcall CBase_dtor(CBase* self) { (void)self; g_base_dtor++; }

static int g_deletes = 0;
void __cdecl op_delete(void* p) { (void)p; g_deletes++; }

struct CPatchTesselationEdgeStrip {
    char pad0[0x50];
    void* p50;
    char pad54[0x8];
    void* p5c;
    char pad60[0x8];
    void* p68;
    char pad6c[0x8];
    void* p74;
    char pad78[0x8];
    void* p80;
    char pad84[0x8];
    CMember m8c;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CMember_dtor(&self->m8c);
    if (self->p80) op_delete(self->p80);
    if (self->p74) op_delete(self->p74);
    if (self->p68) op_delete(self->p68);
    if (self->p5c) op_delete(self->p5c);
    if (self->p50) op_delete(self->p50);
    CBase_dtor((CBase*)self);
}

int main() {
    CPatchTesselationEdgeStrip o;
    int dummy = 1;
    o.p50 = &dummy; o.p5c = &dummy; o.p68 = &dummy; o.p74 = &dummy; o.p80 = &dummy;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_member_dtor == 1 && g_base_dtor == 1 && g_deletes == 5) {
        std::printf("CPatchTesselationEdgeStrip_00455d44_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL m=%d b=%d d=%d\n", g_member_dtor, g_base_dtor, g_deletes);
    return 1;
}