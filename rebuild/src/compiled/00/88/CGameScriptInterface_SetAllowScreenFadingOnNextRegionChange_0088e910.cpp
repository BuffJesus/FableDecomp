struct CMid {
    unsigned char _pad[0xdb];
    unsigned char allowScreenFadingOnNextRegionChange; // +0xdb
};
struct CGameScriptInterface {
    void* vt;      // +0x0
    CMid* mid;     // +0x4
};
void __fastcall CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(
        CGameScriptInterface* self, int edx_dummy, char val) {
    self->mid->allowScreenFadingOnNextRegionChange = (unsigned char)val;
}