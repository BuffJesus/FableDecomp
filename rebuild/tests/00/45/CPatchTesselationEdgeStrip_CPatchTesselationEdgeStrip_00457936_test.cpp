
#include <cstdio>
struct CSub60 { void dtor(); };
struct CBase { void dtor(); };

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x3c];
    void* p3c;
    unsigned char _pad_0x40[0x8];
    void* p48;
    unsigned char _pad_0x4c[0x8];
    void* p54;
    unsigned char _pad_0x58[0x8];
    CSub60 sub60;
};

static int g_sub60=0, g_base=0, g_freed=0;
void CSub60::dtor(){ g_sub60++; }
void CBase::dtor(){ g_base++; }
void __cdecl op_delete(void* p){ if(p) g_freed++; }

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    self->sub60.dtor();
    if (self->p54) op_delete(self->p54);
    if (self->p48) op_delete(self->p48);
    if (self->p3c) op_delete(self->p3c);
    ((CBase*)self)->dtor();
}

int main(){
    CPatchTesselationEdgeStrip o;
    int dummy=0;
    o.p3c=&dummy; o.p48=&dummy; o.p54=&dummy;
    CPatchTesselationEdgeStrip_dtor(&o);
    if(g_sub60!=1||g_base!=1||g_freed!=3){ std::printf("FAIL a\n"); return 1; }
    g_sub60=g_base=g_freed=0;
    o.p3c=0; o.p48=0; o.p54=0;
    CPatchTesselationEdgeStrip_dtor(&o);
    if(g_sub60!=1||g_base!=1||g_freed!=0){ std::printf("FAIL b\n"); return 1; }
    std::printf("CPatchTesselationEdgeStrip_00457936_TEST PASS\n");
    return 0;
}