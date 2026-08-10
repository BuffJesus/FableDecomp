struct GIC_Obj { char pad[0x3c]; long cat; };
extern GIC_Obj* g_gic_ptr;
GIC_Obj* g_gic_ptr = (GIC_Obj*)0;
extern "C" char __fastcall GIC_check(void);
long __fastcall GetInventoryCategory(void)
{
    if (GIC_check())
        return g_gic_ptr->cat;
    return -1;
}