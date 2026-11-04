// CCodeSectionManager::SetEnableSectionLoading @ 0x009F0140
// (Ego_r/FableWin ?SetEnableSectionLoading@CCodeSectionManager@@QAEX_N@Z).
// Retail caller 0x004138C0 passes CTBaseSingleton<CCodeSectionManager>::Get()
// (ecx = [0x013CAA68]); byte +0x28 is SectionLoadingEnabled.
#include "engine/CCodeSectionManager.h"
void CCodeSectionManager::SetEnableSectionLoading(bool enable)
{
    SectionLoadingEnabled = enable;
}
