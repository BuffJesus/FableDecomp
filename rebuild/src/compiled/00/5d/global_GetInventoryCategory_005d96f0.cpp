// GetInventoryCategory @ 005d96f0
struct GObj { char pad[0x6c]; long field_6c; };
extern GObj* g_obj;
extern bool HelperCheck(void);

long __fastcall GetInventoryCategory(long /*unused*/)
{
    if (HelperCheck())
        return g_obj->field_6c;
    return 4;
}