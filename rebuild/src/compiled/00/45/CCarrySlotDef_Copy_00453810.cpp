struct CCarrySlotDef { char pad[4]; };

extern void __fastcall CCarrySlotDef_CopyImpl(CCarrySlotDef* self, void* src);

void __fastcall CCarrySlotDef_Copy(CCarrySlotDef* self, void* src)
{
    CCarrySlotDef_CopyImpl(self, src);
}