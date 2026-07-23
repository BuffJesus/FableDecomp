#pragma optimize("s",on)
extern "C" void __cdecl free_mem(void* p);
struct CBase { int base_pad; };
extern void __fastcall CBase_dtor(CBase* self);
struct CPatchTesselationEdgeStrip {
    char pad[0x68];
    void* p68;   /* +0x68 */
    char pad2[0x74 - 0x68 - 4];
    void* p74;   /* +0x74 */
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p74)
        free_mem(self->p74);
    if (self->p68)
        free_mem(self->p68);
    CBase_dtor((CBase*)self);
}