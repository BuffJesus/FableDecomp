struct CJackDragonDef { char pad[4]; };
extern void __fastcall CJackDragonDef_CopyImpl(CJackDragonDef* self, void* dummy, const CJackDragonDef* src);

void __fastcall CJackDragonDef_Copy(CJackDragonDef* self, void* dummy, const CJackDragonDef* src)
{
    CJackDragonDef_CopyImpl(self, dummy, src);
}