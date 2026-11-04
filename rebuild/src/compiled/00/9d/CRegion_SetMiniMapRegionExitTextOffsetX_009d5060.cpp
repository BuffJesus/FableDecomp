// CDefString::SetString(const CCharString&) @ 0x009D5060 (FableWin
// ?SetString@CDefString@@QAEXABVCCharString@@@Z; the inline
// CDefString(const CCharString&) is this call, e.g. in
// CDefinitionManager::InstantiateDef 0x009ADC90). Retyped from a wrongly
// named CRegion::SetMiniMapRegionExitTextOffsetX landing.
#include "fable_def_string.h"

// Intern the text in the global def string table and keep its index.
void CDefString::SetString(const CCharString& text)
{
    TablePos = GDefStringTable.AddString(text);
}