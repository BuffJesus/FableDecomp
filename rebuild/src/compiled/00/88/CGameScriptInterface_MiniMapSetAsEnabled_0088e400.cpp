struct Inner { void SetEnabled(bool b); };
struct CGameScriptInterface { void* pad0; Inner* inner; };

void __fastcall CGameScriptInterface_MiniMapSetAsEnabled(CGameScriptInterface* self, void* edx, bool enabled)
{
    (void)edx;
    self->inner->SetEnabled(enabled);
}