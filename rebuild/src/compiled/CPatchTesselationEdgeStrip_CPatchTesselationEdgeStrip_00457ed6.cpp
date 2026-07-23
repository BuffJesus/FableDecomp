#pragma optimize("s",on)
extern "C" void __cdecl eng_free(void* p);
extern void __fastcall CBase_dtor(void* self);

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x48];
    void* p48;   // +0x48
    unsigned char _pad_0x4c[0x8];
    void* p54;   // +0x54
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54)
        eng_free(self->p54);
    if (self->p48)
        eng_free(self->p48);
    CBase_dtor(self);
}