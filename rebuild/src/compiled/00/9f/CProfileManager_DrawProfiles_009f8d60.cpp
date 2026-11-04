// CProfileManager::DrawProfiles(const C2DBoxF&, const CFontBank&) @ 009f8d60 (3 bytes: ret 8)
//
// Profiling is compiled out of retail, so the body is empty. Identity: it
// closes the retail run that mirrors Ego_r's CProfileManager member order
// (... SwitchEvalMode 0x009F8D30 toggling +0x44, ResetMinFrame 0x009F8D40,
// SaveProfiles(CWideString) 0x009F8D50 destroying its by-value argument, then
// DrawProfiles), and its sole caller 0x00435783 pushes two references and
// loads CSystemManager+0xF8 (the CCountedPointer<CProfileManager>) into ecx.
// Needs the declaration added to CProfileManager in fable_system_manager.h;
// this directory's copy of that header is the test overlay.

#include "fable_system_manager.h"

void CProfileManager::DrawProfiles(const C2DBoxF& box, const CFontBank& font)
{
}
