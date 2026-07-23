struct CSnowTrollDef;
extern void __fastcall CSnowTrollDef_CopyImpl(CSnowTrollDef* self, const CSnowTrollDef* other);
void __fastcall CSnowTrollDef_Copy(CSnowTrollDef* self, const CSnowTrollDef* other)
{
    CSnowTrollDef_CopyImpl(self, other);
}