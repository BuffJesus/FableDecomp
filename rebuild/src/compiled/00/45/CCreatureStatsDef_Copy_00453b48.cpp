struct CCreatureStatsDef;
extern void __fastcall CCreatureStatsDef_CopyImpl(CCreatureStatsDef* self, void* dummy, const CCreatureStatsDef* src);

void __fastcall CCreatureStatsDef_Copy(CCreatureStatsDef* self, void* dummy, const CCreatureStatsDef* src)
{
    CCreatureStatsDef_CopyImpl(self, dummy, src);
}