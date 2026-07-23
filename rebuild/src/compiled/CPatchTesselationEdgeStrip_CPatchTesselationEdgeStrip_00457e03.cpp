#pragma optimize("s",on)
// ~CPatchTesselationEdgeStrip
struct CMember { void* v; void __fastcall dtor(); };

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x3c];
    void* p3c;      // +0x3c
    unsigned char _pad_0x40[0x18]; // to 0x58
    void* p58;      // +0x58
    unsigned char _pad_0x5c[0x8];  // to 0x64
    CMember m64;    // +0x64
    CMember m68;    // +0x68
};

extern void __cdecl engine_free(void*);
extern void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip*);

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    self->m68.dtor();
    self->m64.dtor();
    if (self->p58) engine_free(self->p58);
    if (self->p3c) engine_free(self->p3c);
    CPatchTesselationEdgeStrip_base_dtor(self);
}