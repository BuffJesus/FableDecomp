// CGameDefinitionManager::Set, retail 0x0044c71f (FableWin ?Set@CGameDefinitionManager@@SIXPAV1@@Z): hand the singleton pointer
// to CGameDefinitionManager::PClass (CCountedPointer::Reset takes ownership).
#include "fable_definition_manager.h"

void CGameDefinitionManager::Set(CGameDefinitionManager* manager)
{
    PClass.Reset(manager);
}