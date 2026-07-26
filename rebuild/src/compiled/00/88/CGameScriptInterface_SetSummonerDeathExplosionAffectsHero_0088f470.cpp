struct CMid { unsigned char _pad[0xe1]; unsigned char flagE1; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(CGameScriptInterface* self, int /*edx*/, char v)
{
    self->mid->flagE1 = v;
}