#pragma optimize("s",on)
struct CSub { };
extern void __fastcall CSub_dtor(CSub* self);
extern void __cdecl op_delete(void* p);
extern void __fastcall CBase_dtor(void* self);

struct CPatchTesselationEdgeStrip {
    char pad0[0x60];
    void* p60;
    char pad1[0x08];
    void* p6c;
    char pad2[0x08];
    void* p78;
    char pad3[0x08];
    void* p84;
    char pad4[0x08];
    CSub sub90;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CSub_dtor(&self->sub90);
    if (self->p84) op_delete(self->p84);
    if (self->p78) op_delete(self->p78);
    if (self->p6c) op_delete(self->p6c);
    if (self->p60) op_delete(self->p60);
    CBase_dtor(self);
}