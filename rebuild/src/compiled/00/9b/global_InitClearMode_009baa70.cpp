// CPersistContext::InitClearMode() @ 0x009BAA70 (FableWin
// ?InitClearMode@CPersistContext@@QAEXXZ): the defaults mode, with no
// definition manager and no CRC check (CDefinitionManager::AddDefClass
// 0x009B0AC0 clears a new def through it).
#include "fable_persist_context.h"

void CPersistContext::InitClearMode()
{
    Mode = MODE_DEFAULTS;
    PDefinitionManager = 0;
    UseCRCCheck = false;
}
