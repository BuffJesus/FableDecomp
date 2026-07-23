struct CTattooDef;
extern void __fastcall CTattooDef_CopyImpl(CTattooDef* self, void* edx, const CTattooDef* src);

void __fastcall CTattooDef_Copy(CTattooDef* self, void* edx, const CTattooDef* src)
{
    CTattooDef_CopyImpl(self, edx, src);
}