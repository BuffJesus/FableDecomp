#pragma optimize("s",on)
struct CPatchTesselationEdgeStrip {
    char pad0[0x3c];
    void* p3c;
    char pad1[0x48 - 0x3c - 4];
    void* p48;
};

extern "C" void __fastcall BaseDtor(CPatchTesselationEdgeStrip* self);

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p48)
        operator delete(self->p48);
    if (self->p3c)
        operator delete(self->p3c);
    BaseDtor(self);
}