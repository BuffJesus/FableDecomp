struct Mid { unsigned char pad[0xd5]; unsigned char teleporting; };
struct CGameScriptInterface { void* vt; Mid* mid; };
void __fastcall CGameScriptInterface_SetTeleportingAsActive(CGameScriptInterface* self, int edx, char active) {
    self->mid->teleporting = active;
}