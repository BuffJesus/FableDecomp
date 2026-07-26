struct CountdownSub { char pad[0xa0]; int frames; };
struct CGameScriptInterface { void* pad0; CountdownSub* sub; };

float __fastcall CGameScriptInterface_GetCountdownTimer(const CGameScriptInterface* self)
{
    return (float)self->sub->frames;
}