#pragma optimize("s",on)
struct CInventoryCategoryDef { char pad[0x58]; };

unsigned long __fastcall CInventoryCategoryDef_GetSizeofClass(CInventoryCategoryDef* self)
{
    (void)self;
    return 0x58;
}