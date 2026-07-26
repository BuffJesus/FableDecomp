#pragma optimize("s",on)
extern "C" void engine_free(void*);
struct CBase { };
extern void __fastcall CBase_dtor(CBase*);
struct CPatchTesselationEdgeStrip {
    unsigned char _pad0[0x3c];
    void* p3c;   // +0x3c
    unsigned char _pad2[0x48-0x40];
    void* p48;   // +0x48
    unsigned char _pad3[0x54-0x4c];
    void* p54;   // +0x54
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54) engine_free(self->p54);
    if (self->p48) engine_free(self->p48);
    if (self->p3c) engine_free(self->p3c);
    CBase_dtor((CBase*)self);
}