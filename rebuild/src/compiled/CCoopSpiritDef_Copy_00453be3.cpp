struct CCoopSpiritDef;
extern void __fastcall Base_Copy(CCoopSpiritDef* self, const CCoopSpiritDef* other);

void __fastcall CCoopSpiritDef_Copy(CCoopSpiritDef* self, const CCoopSpiritDef* other)
{
    Base_Copy(self, other);
}