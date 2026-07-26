struct CTeleportState { char pad[0xd5]; bool active; };
struct CGameScriptInterface { void* vtbl; CTeleportState* state; };

bool __fastcall CGameScriptInterface_IsTeleportingActive(const CGameScriptInterface* self)
{
    return self->state->active;
}