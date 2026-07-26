#include "rebuild_abi.h"
#include <cstdio>

struct CPatchTesselationEdgeStrip { char _pad[0x6c + 4]; };
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr);

static int g_deletes = 0;
static int g_base = 0;

extern "C" void __cdecl engine_op_delete(void* p) { if (p) g_deletes++; }
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip*) { g_base++; }

int main()
{
    CPatchTesselationEdgeStrip obj;
    char* b = (char*)&obj;
    for (unsigned i = 0; i < sizeof(obj); i++) b[i] = 0;

    int v = 1;
    *(void**)(b + 0x6c) = &v;
    *(void**)(b + 0x60) = 0;
    *(void**)(b + 0x54) = &v;
    *(void**)(b + 0x48) = 0;
    *(void**)(b + 0x3c) = &v;

    CPatchTesselationEdgeStrip_dtor(&obj);

    if (g_deletes == 3 && g_base == 1)
        printf("OK_458476\n");
    else
        printf("FAIL %d %d\n", g_deletes, g_base);
    return 0;
}