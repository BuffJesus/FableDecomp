struct Inner { char pad[0xd9]; unsigned char flag; };
struct CGameScriptInterface { void* vtbl; Inner* inner; };

unsigned char __fastcall CGameScriptInterface_IsHeroSleepingEnabled(const CGameScriptInterface* self)
{
    return self->inner->flag;
}