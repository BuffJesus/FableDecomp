// CExplosiveTrailDef::Copy - tail-call thunk forwarding to base Copy
struct CExplosiveTrailDef;
extern void __fastcall CExplosiveTrailDef_CopyImpl(CExplosiveTrailDef* self, const void* src);

void __fastcall CExplosiveTrailDef_Copy(CExplosiveTrailDef* self, const void* src)
{
    CExplosiveTrailDef_CopyImpl(self, src);
}