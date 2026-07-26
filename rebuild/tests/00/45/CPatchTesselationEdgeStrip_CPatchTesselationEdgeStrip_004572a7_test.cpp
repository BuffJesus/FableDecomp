#include <cstdio>
static int g_freed = 0;
static void* g_last = 0;
extern "C" void free_mem(void* p) { g_freed++; g_last = p; }
static int g_basedtor = 0;
struct CBase { int base_pad; };
void CBase_dtor(CBase* self) { (void)self; g_basedtor++; }
struct CPatchTesselationEdgeStrip {
    char pad[0x68];
    void* p68;
    char pad2[0x74 - 0x68 - 4];
    void* p74;
};
void CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p74)
        free_mem(self->p74);
    if (self->p68)
        free_mem(self->p68);
    CBase_dtor((CBase*)self);
}
int main() {
    CPatchTesselationEdgeStrip o;
    int dummy1, dummy2;
    o.p68 = &dummy1;
    o.p74 = &dummy2;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed != 2) { std::printf("FAIL freed=%d\n", g_freed); return 1; }
    if (g_basedtor != 1) { std::printf("FAIL basedtor=%d\n", g_basedtor); return 1; }
    g_freed = 0; g_basedtor = 0;
    o.p68 = 0; o.p74 = 0;
    CPatchTesselationEdgeStrip_dtor(&o);
    if (g_freed != 0) { std::printf("FAIL freed2=%d\n", g_freed); return 1; }
    if (g_basedtor != 1) { std::printf("FAIL basedtor2=%d\n", g_basedtor); return 1; }
    std::printf("CPatchTesselationEdgeStrip_004572a7_TEST PASS\n");
    return 0;
}