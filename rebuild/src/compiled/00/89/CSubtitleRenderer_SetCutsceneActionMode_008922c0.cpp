#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString;
struct Inner { void SetMode(bool, const CCharString&); };
extern Inner* g_cutscene;
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void SetCutsceneActionMode(bool, const CCharString&) const;
};
void CGameScriptInterface_Methods::SetCutsceneActionMode(bool a, const CCharString& b) const {
    ((Inner*)((char*)g_cutscene + 0x18))->SetMode(a, b);
}