struct CMid { unsigned char _pad[0xdf]; unsigned char regionTextActive; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetRegionTextDisplayAsActive(CGameScriptInterface* self, void* edx, char active)
{
    self->mid->regionTextActive = (unsigned char)active;
}