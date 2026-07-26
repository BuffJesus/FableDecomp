struct CGameScriptInterface {
    void* vt;
};

extern unsigned char g_boastingEnabled;

void __fastcall CGameScriptInterface_SetBoastingEnabled(CGameScriptInterface* self, void* edx, bool enabled)
{
    (void)self; (void)edx;
    g_boastingEnabled = (unsigned char)enabled;
}