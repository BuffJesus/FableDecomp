// CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes @ 0x00890b50
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CMainGameComponent {
    char pad[0x1c];
    void* f1c;      // +0x1c
};

// helper @ 0x449970 : __fastcall(void* that) -> void* (object)
void* __fastcall helper_449970(void* that);

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int /*edx*/, bool keep)
{
    unsigned char* obj = (unsigned char*)helper_449970(self->Component->f1c);
    obj[0x224] = (unsigned char)keep;
}