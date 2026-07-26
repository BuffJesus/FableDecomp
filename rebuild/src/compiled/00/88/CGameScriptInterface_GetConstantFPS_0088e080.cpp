struct CGameScriptInterface;
extern long g_constantFPS;

long __fastcall CGameScriptInterface_GetConstantFPS(const CGameScriptInterface* self)
{
    (void)self;
    return g_constantFPS;
}