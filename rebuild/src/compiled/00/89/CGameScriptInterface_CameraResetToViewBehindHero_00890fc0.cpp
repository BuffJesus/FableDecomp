extern int g_someInt;

struct CGameScriptInterface {
    char pad[0x48];
    int field48;
};

void __fastcall CGameScriptInterface_CameraResetToViewBehindHero(CGameScriptInterface* self, void* edx, float scale)
{
    long v = (long)((double)g_someInt * scale);
    self->field48 = v;
    if (v < 0) self->field48 = 0;
}