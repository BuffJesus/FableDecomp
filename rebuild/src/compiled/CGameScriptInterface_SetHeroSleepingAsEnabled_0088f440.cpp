struct CMid { unsigned char _pad[0xd9]; unsigned char heroSleeping; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetHeroSleepingAsEnabled(CGameScriptInterface* self, int edx_unused, char enabled)
{
    self->mid->heroSleeping = enabled;
}