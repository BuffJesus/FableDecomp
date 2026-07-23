#pragma optimize("s",on)
struct CPatchTesselationEdgeStrip {
    unsigned char _pad0[0x78];
    void* p78;   // +0x78
    unsigned char _pad1[0x8c-0x7c];
    void* p8c;   // +0x8c
    unsigned char _pad2[0x98-0x90];
    void* p98;   // +0x98
};

extern "C" void __cdecl EdgeStrip_freefn(void* p);
extern void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* self);

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p98) EdgeStrip_freefn(self->p98);
    if (self->p8c) EdgeStrip_freefn(self->p8c);
    if (self->p78) EdgeStrip_freefn(self->p78);
    CPatchTesselationEdgeStrip_base_dtor(self);
}