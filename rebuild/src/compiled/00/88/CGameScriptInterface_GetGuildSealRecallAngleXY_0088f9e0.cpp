struct Inner { char pad[0x160]; float angle; };
struct CGameScriptInterface { void* vtbl; Inner* inner; };

float __fastcall CGameScriptInterface_GetGuildSealRecallAngleXY(const CGameScriptInterface* self)
{
    return self->inner->angle;
}