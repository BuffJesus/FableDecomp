struct CMid { unsigned char _pad[0xd6]; unsigned char flag; };
struct CGameScriptInterface { void* vt; CMid* mid; };
void __fastcall CGameScriptInterface_SetExperienceSpendingAsEnabled(CGameScriptInterface* self, void* /*edx*/, unsigned char enabled) {
    self->mid->flag = enabled;
}