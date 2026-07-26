#include <cstdio>
#include "rebuild_abi.h"

struct CPatchTesselationEdgeStrip;
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr);

static int g_freed = 0;
extern "C" void __cdecl j_operator_delete(void* p) { if (p) ++g_freed; }

struct CPatchTesselationEdgeStrip {
    char pad00[0x3c];
    void* p3c;
    char pad40[0x48 - 0x40];
    void* p48;
    char pad4c[0x54 - 0x4c];
    void* p54;
    char pad58[0x60 - 0x58];
    void* p60;
    char pad64[0x6c - 0x64];
    void* p6c;
};

static int g_base = 0;
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip*) { ++g_base; }

int main() {
    CPatchTesselationEdgeStrip obj;
    obj.p3c = (void*)1;
    obj.p48 = (void*)2;
    obj.p54 = 0;          // exercise the null-skip path
    obj.p60 = (void*)4;
    obj.p6c = (void*)5;
    CPatchTesselationEdgeStrip_dtor(&obj);
    if (g_freed == 4 && g_base == 1) {
        printf("OK_00457ae4_DTOR\n");
    } else {
        printf("FAIL freed=%d base=%d\n", g_freed, g_base);
    }
    return 0;
}