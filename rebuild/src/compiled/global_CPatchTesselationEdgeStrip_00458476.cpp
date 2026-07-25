#pragma optimize("s",on)
#include "rebuild_abi.h"

// operator delete (relocation-masked target)
extern "C" void __cdecl engine_op_delete(void* p);

// base-class destructor tail-called at end (relocation-masked target)
struct CPatchTesselationEdgeStrip;
extern void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* thisptr);

struct CPatchTesselationEdgeStrip {
    char _pad[0x6c + 4];
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr)
{
    void* p;

    p = *(void**)((char*)thisptr + 0x6c);
    if (p) engine_op_delete(p);

    p = *(void**)((char*)thisptr + 0x60);
    if (p) engine_op_delete(p);

    p = *(void**)((char*)thisptr + 0x54);
    if (p) engine_op_delete(p);

    p = *(void**)((char*)thisptr + 0x48);
    if (p) engine_op_delete(p);

    p = *(void**)((char*)thisptr + 0x3c);
    if (p) engine_op_delete(p);

    CPatchTesselationEdgeStrip_base_dtor(thisptr);
}