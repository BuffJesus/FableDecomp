#pragma optimize("s",on)
#include "rebuild_abi.h"

// operator delete (retail 0x7a6f30) — relocation-masked call target
extern "C" void __cdecl j_operator_delete(void* p);

struct CPatchTesselationEdgeStrip;
// base destructor tail-called at end (relocation-masked target)
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* thisptr);

struct CPatchTesselationEdgeStrip {
    char pad00[0x3c];
    void* p3c;   // +0x3c
    char pad40[0x48 - 0x40];
    void* p48;   // +0x48
    char pad4c[0x54 - 0x4c];
    void* p54;   // +0x54
    char pad58[0x60 - 0x58];
    void* p60;   // +0x60
    char pad64[0x6c - 0x64];
    void* p6c;   // +0x6c
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr)
{
    if (thisptr->p6c) { j_operator_delete(thisptr->p6c); }
    if (thisptr->p60) { j_operator_delete(thisptr->p60); }
    if (thisptr->p54) { j_operator_delete(thisptr->p54); }
    if (thisptr->p48) { j_operator_delete(thisptr->p48); }
    if (thisptr->p3c) { j_operator_delete(thisptr->p3c); }
    CPatchTesselationEdgeStrip_base_dtor(thisptr);
}