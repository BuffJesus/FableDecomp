struct CMid { unsigned char pad[0xd7]; unsigned char flagD7; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetSavingAsEnabled(CGameScriptInterface* self, int edx_dummy, char enabled)
{
    self->mid->flagD7 = (unsigned char)enabled;
}