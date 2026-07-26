struct CThing;
struct CGameScriptInterface { char pad[0x14]; void* field14; };

extern void* __fastcall Sub_GetManager(void* self);
extern CThing* __fastcall Sub_ResolveHero(void* mgr);
extern float __fastcall Sub_GetHealthPct(CThing* hero);
extern float g_defaultHealthPct;

float __fastcall CGameScriptInterface_GetHeroHealthPercentage(CGameScriptInterface* self)
{
    void* mgr = self->field14;
    CThing* hero = Sub_ResolveHero(Sub_GetManager(mgr));
    if (hero != 0 && (*(unsigned char*)((char*)hero + 0x91) & 0x1) == 0) {
        float v = Sub_GetHealthPct(hero);
        return v;
    }
    return g_defaultHealthPct;
}