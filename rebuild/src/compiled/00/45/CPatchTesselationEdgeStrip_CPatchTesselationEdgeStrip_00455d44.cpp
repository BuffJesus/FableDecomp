#pragma optimize("s",on)
struct CMember { void dtor(); };
extern void __fastcall CMember_dtor(CMember* self);

struct CBase { void dtor(); };
extern void __fastcall CBase_dtor(CBase* self);

extern void __cdecl op_delete(void* p);

struct CPatchTesselationEdgeStrip {
    char pad0[0x50];
    void* p50;
    char pad54[0x8];
    void* p5c;
    char pad60[0x8];
    void* p68;
    char pad6c[0x8];
    void* p74;
    char pad78[0x8];
    void* p80;
    char pad84[0x8];
    CMember m8c;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CMember_dtor(&self->m8c);
    if (self->p80) op_delete(self->p80);
    if (self->p74) op_delete(self->p74);
    if (self->p68) op_delete(self->p68);
    if (self->p5c) op_delete(self->p5c);
    if (self->p50) op_delete(self->p50);
    CBase_dtor((CBase*)self);
}