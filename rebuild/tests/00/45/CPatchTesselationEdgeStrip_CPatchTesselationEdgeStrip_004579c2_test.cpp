#include <cstdio>
extern "C" void engine_free(void*);
struct CBase { };
void __fastcall CBase_dtor(CBase*);
struct CPatchTesselationEdgeStrip {
    unsigned char _pad0[0x3c];
    void* p3c;
    unsigned char _pad2[0x48-0x40];
    void* p48;
    unsigned char _pad3[0x54-0x4c];
    void* p54;
};
static int g_freed = 0;
static void* g_last[8];
extern "C" void engine_free(void* p){ g_last[g_freed++] = p; }
static int g_baseDtor = 0;
void __fastcall CBase_dtor(CBase*){ g_baseDtor++; }
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54) engine_free(self->p54);
    if (self->p48) engine_free(self->p48);
    if (self->p3c) engine_free(self->p3c);
    CBase_dtor((CBase*)self);
}
int main(){
    CPatchTesselationEdgeStrip o;
    for (unsigned i=0;i<sizeof(o);++i) ((unsigned char*)&o)[i]=0;
    o.p54 = (void*)0x1000;
    o.p48 = 0;
    o.p3c = (void*)0x3000;
    g_freed=0; g_baseDtor=0;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed==2 && g_last[0]==(void*)0x1000 && g_last[1]==(void*)0x3000 && g_baseDtor==1) {
        std::printf("CPatchTesselationEdgeStrip_004579c2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL freed=%d base=%d\n", g_freed, g_baseDtor);
    return 1;
}