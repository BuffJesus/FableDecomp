struct CGameScriptInterface {
    void* vt; // +0x0
    unsigned char _pad[0x29]; // to 0x2d
    char allowFadeIfFaded; // +0x2d
};

void __fastcall CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded(CGameScriptInterface* self, int /*edx*/, char val)
{
    self->allowFadeIfFaded = val;
}